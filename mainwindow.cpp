#include "mainwindow.h"
#include <QDir>
#include <queue>
//Pixel size of each Tile in the grid
const int tileLen = 32;
//Data structure that stores the location of each tile in the grid
//0 = empty, 1 = wall, 2 = player, 3 = enemy
int grid[15][20];

// Flag to track whether the settings screen is currently open
bool settingsOpen = false;
int difficulty = 0;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//Create the scene object and set the scene to the view
	createScene();
	setCentralWidget(view);
	setWindowTitle("2D Shooter");
	music.playSound("bgm");

	//Timers
	invincTimer = new QTimer(this);
	invincTimer->setSingleShot(true);
	enemyTimer = new QTimer(this);
	tempTimer = new QTimer(this);

	//Connections:
	connect(&player, SIGNAL(drawUi()), this, SLOT(drawUI()));
	connect(&player, SIGNAL(openSettings()), this, SLOT(settings()));
	connect(this, SIGNAL(enemyAttack()), &enemies[0], SLOT(attackHandler()));
	connect(this, SIGNAL(enemyAttack()), &enemies[1], SLOT(attackHandler()));
	connect(&player, SIGNAL(collisionHandler()), this, SLOT(collisionHandler()));
	connect(invincTimer, &QTimer::timeout, this, [&]() {
		player.setStatus(Player::normal);
	//remove invincibility shadoweffect
	player.setGraphicsEffect(nullptr);
	if (tempTimer != nullptr) {
		tempTimer->stop();
	}
		});
	connect(invincTimer, SIGNAL(timeout()), this, SLOT(drawUI()));
	//Connect the enemytimer to lambda that calls pathfinding
	connect(enemyTimer, &QTimer::timeout, this, [&]() {
		if (scene->items().contains(&enemies[1]) && scene->items().contains(&enemies[0])) {
			pathFinding(enemies[0]);
			pathFinding(enemies[1]);
		}
		else if (scene->items().contains(&enemies[0])) {
			pathFinding(enemies[0]);
		}
		else if (scene->items().contains(&enemies[1])) {
			pathFinding(enemies[1]);
		}
		});

	drawScene();
	drawUI();
	enemyTimer->start(1000);
}

MainWindow::~MainWindow()
{
	delete scene;
	delete view;
}

void MainWindow::addTile(int x, int y, QString tileName) {
	QGraphicsPixmapItem* newItem = new QGraphicsPixmapItem;
	//Set pixmap from .qrc file and make sure it is scaled
	newItem->setPixmap(QPixmap(":/tiles/" + tileName).scaled(tileLen, tileLen));
	newItem->setPos(x * tileLen, y * tileLen);

	// Check if the scene already contains this item before adding it
	if (!scene->items().contains(newItem)) {
		scene->addItem(newItem);
	}
}

void MainWindow::drawScene() {
	hearts.clear();
	for (auto item : scene->items()) {
		if (typeid(*item) == typeid(Collectibles)) {
			dynamic_cast<Collectibles*>(item)->group->stop();
		}
	}
	scene->clear();
	//Initialize the grid
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 20; j++) {
			grid[i][j] = 0;
		}
	}
	//Read the tmx file then go line by line in the csv
	QFile file(":/maps/level" + QString::number(level));
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
	QString line = in.readLine();
	while (!line.isNull()) {
		if (line.contains("<data encoding=\"csv\">")) {
			line = in.readLine();
			int y = 0;
			while (!line.isNull()) {
				QStringList list = line.split(",");
				for (int x = 0; x < list.size(); x++) {
					//If the tile is not empty, add it to the scene
					grid[y][x] = list[x].toInt();
				}
				y++;
				line = in.readLine();
			}
		}
		line = in.readLine();
	}
	graph = new Graph();

	//Use the list to add the tiles to the scene the name of the tile is also its id
	//Ex: if tile name is "1" then it should be placed wherever there is 1 in the grid
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 20; j++) {
			if (grid[i][j] != 0) {
				addTile(j, i, QString::number(grid[i][j]));
			}
		}
	}

	//Add player to the middle of the scene
	player.setPos(420, 200);
	player.setFocus();
	scene->addItem(&player);
	//Add enemy to the middle of the scene
	enemies[0].setPos(150, 120);
	scene->addItem(&enemies[0]);
	enemies[1].setPos(400, 380);
	scene->addItem(&enemies[1]);

	//Check if the scene already contains the heart

	for (int i = 0; i < player.getHealth(); i++) {
		QGraphicsPixmapItem* heart = new QGraphicsPixmapItem;
		heart->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
		heart->setPos(0 + (i * tileLen), 0);
		scene->addItem(heart);
		hearts.push_back(heart);
	}

	object = new Collectibles(Collectibles::heart);
	object->setParent(this);
	object->setPos(500, 120);
	scene->addItem(object);
	object->animHandler();

	object = new Collectibles(Collectibles::bullet);
	object->setParent(this);
	object->setPos(60, 400);
	scene->addItem(object);
	object->animHandler();

	object = new Collectibles(Collectibles::bullet);
	object->setParent(this);
	object->setPos(120, 100);
	scene->addItem(object);
	object->animHandler();

	object = new Collectibles(Collectibles::bullet);
	object->setParent(this);
	object->setPos(400, 60);
	scene->addItem(object);
	object->animHandler();

	object = new Collectibles(Collectibles::bullet);
	object->setParent(this);
	object->setPos(550, 300);
	scene->addItem(object);
	object->animHandler();

	object = new Collectibles(Collectibles::shield);
	object->setParent(this);
	object->setPos(300, 400);
	scene->addItem(object);
	object->animHandler();

	object = new Collectibles(Collectibles::exit);
	object->setParent(this);
	object->setPos(550, 400);
	//Check if the item is already added to the scene and if its the same scene
	if (!scene->items().contains(object) && scene == view->scene()) {
		scene->addItem(object);
	}
	object->animHandler();
}

//Function that controls the ui
void MainWindow::drawUI() {
	// Update hearts
	if (player.getHealth() > hearts.size()) {
		QGraphicsPixmapItem* heart = new QGraphicsPixmapItem;
		heart->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
		heart->setPos(0 + (hearts.size() * tileLen), 0);
		scene->addItem(heart);
		hearts.push_back(heart);
	}
	else if (player.getHealth() < hearts.size()) {
		hearts[player.getHealth()]->setPixmap(QPixmap(":/ui/emptyHeart").scaled(tileLen, tileLen));
		hearts.pop_back();
	}

	if (player.getHealth() == 0) {
		scene->removeItem(&player);
		gameOver();
	}

	for (int i = 0; i < 2; i++) {
		Enemy& enemy = enemies[i];
		if (enemy.getHealth() == 0) {
			//Timer connected to enemy deathHandler
			QTimer* deathTimer = new QTimer;
			deathTimer->start(300);
			connect(deathTimer, SIGNAL(timeout()), &enemy, SLOT(deathHandler()), Qt::QueuedConnection);
			QTimer::singleShot(1000, this, [this, &enemy, deathTimer]() {
				scene->removeItem(&enemy);
			deathTimer->stop();
				});
		}
	}

	//Only add it once to the scene
	if (scene->items().contains(ammo) == false) {
		//Show the bullet count
		ammo = new QGraphicsTextItem;
		ammo->setPlainText(QString("Bullets: ") + QString::number(player.getAmmo()));
		ammo->setDefaultTextColor(Qt::white);
		ammo->setFont(QFont("times", 16));
		ammo->setPos(0, 50);
		scene->addItem(ammo);
	}
	else {
		ammo->setPlainText(QString("Bullets: ") + QString::number(player.getAmmo()));
	}

	//Show a counter displaying the timer if invincible
	if (player.getStatus() == Player::normal) {
		if (scene->items().contains(statusText) == true) {
			scene->removeItem(statusText);
		}
		statusText = new QGraphicsTextItem;
		statusText->setPlainText(QString("Normal"));
		statusText->setDefaultTextColor(Qt::white);
		statusText->setFont(QFont("times", 16));
		statusText->setPos(0, 100);
		scene->addItem(statusText);
	}
	else if (player.getStatus() == Player::invincible) {
		tempTimer->start(1000);
		//Put a blue particles effect on the player
		player.setGraphicsEffect(new QGraphicsBlurEffect);
		//Connect the timer to the lambda
		connect(tempTimer, &QTimer::timeout, this, [=]() {
			//Update the text make sure its showing a second less each time
			statusText->setPlainText(QString("Invincible: ") + QString::number(invincTimer->remainingTime() / 1000));
			});
	}
}

//collisionHandler
void MainWindow::collisionHandler() {
	//Check if the player is colliding with any enemy
	QList<QGraphicsItem*> colliding_items = player.collidingItems();
	if (scene != nullptr) {
		for (int i = 0; i < colliding_items.size(); i++) {
			//If invinctimer is active dont allow collision with enemy
			QGraphicsItem* item = colliding_items[i];
			if (typeid(*item) == typeid(Enemy)) {
				if (!invincTimer->isActive()) {
					player.setHealth(player.getHealth() - 1);
					player.setPixmap(QPixmap("a"));
					drawUI();
					emit enemyAttack();
					music.playSound("hurt");
					//get the direction of the player
					int dir = player.getDir();
					//move the player back
					if (dir == 0) {
						player.setPos(player.x() - tileLen, player.y());
					}
					else if (dir == 1) {
						player.setPos(player.x() + tileLen, player.y());
					}
					else if (dir == 2) {
						player.setPos(player.x(), player.y() + tileLen);
					}
					else if (dir == 3) {
						player.setPos(player.x(), player.y() - tileLen);
					}
				}
			}
			//if its colliding with a collectible, remove it from the scene and decide its type
			else if (typeid(*item) == typeid(Collectibles)) {
				//If its a bullet increase the players ammo
				if (dynamic_cast<Collectibles*>(colliding_items[i])->getType() == Collectibles::heart) {
					scene->removeItem(colliding_items[i]);
					player.setHealth(player.getHealth() + 1);
					drawUI();
					player.pickUp();
				}
				if (dynamic_cast<Collectibles*>(colliding_items[i])->getType() == Collectibles::bullet) {
					scene->removeItem(colliding_items[i]);
					music.playSound("gunShot");
					player.setAmmo(player.getAmmo() + 1);
					//Euclidean algorithim to find the closest enemy
					int enemy1 = sqrt(pow(player.x() - enemies[0].x(), 2) + pow(player.y() - enemies[0].y(), 2));
					int enemy2 = sqrt(pow(player.x() - enemies[1].x(), 2) + pow(player.y() - enemies[1].y(), 2));
					//If the first enemy is closer then the second enemy
					if (enemy1 < enemy2 && enemies[0].getHealth() != 0) {
						//Shoot at the first enemy
						enemies[0].loseHealth();
						enemies[0].setPixmap(QPixmap(":/enemy1/hurt"));
					}
					else {
						//Shoot at the second enemy
						enemies[1].loseHealth();
						enemies[1].setPixmap(QPixmap(":/enemy1/hurt"));
					}
					drawUI();
					player.pickUp();
				}
				if (dynamic_cast<Collectibles*>(colliding_items[i])->getType() == Collectibles::shield) {
					scene->removeItem(colliding_items[i]);
					invincTimer->start(10000);
					player.setStatus(Player::invincible);
					drawUI();
					player.pickUp();
				}
				if (dynamic_cast<Collectibles*>(colliding_items[i])->getType() == Collectibles::exit) {
					//Check if the exit has already been removed
						//Check the health of both enemies if both of them are dead then the player wins
					if (enemies[0].getHealth() == 0 && enemies[1].getHealth() == 0) {
						if (level == 2) {
							win();
						}
						else {
							level++;
							restart();
						}
					}
				}
			}
		}
	}
}

void MainWindow::enemyCollisionHandler() {
	//Check if the enemies are colliding with a player
	for (int i = 0; i < 2; i++) {
		QList<QGraphicsItem*> colliding_items = enemies[i].collidingItems();
		for (int j = 0; j < colliding_items.size(); j++) {
			if (typeid(*(colliding_items[j])) == typeid(Player)) {
				//If invinctimer is active dont allow collision with enemy
				if (!invincTimer->isActive()) {
					player.setHealth(player.getHealth() - 1);
					player.setPixmap(QPixmap("a"));
					drawUI();
					music.playSound("hurt");
					//get the direction of the player
					int dir = player.getDir();
					//move the player back
					if (dir == 0) {
						player.setPos(player.x() - tileLen, player.y());
					}
					else if (dir == 1) {
						player.setPos(player.x() + tileLen, player.y());
					}
					else if (dir == 2) {
						player.setPos(player.x(), player.y() + tileLen);
					}
					else if (dir == 3) {
						player.setPos(player.x(), player.y() - tileLen);
					}
				}
			}
		}
	}
}

//Creation and setting of scene and view
void MainWindow::createScene() {
	scene = new QGraphicsScene;
	view = new SceneView(this);
	view->setScene(scene);
}

//Size changes
void MainWindow::setSize() {
	this->setFixedSize(960, 720);
	int x = ((screenGeometry.width() - this->width()) / 2);
	int y = ((screenGeometry.height() - this->height()) / 2);
	this->move(x, y);
}

void MainWindow::restart() {
	scene->removeItem(&player);
	scene->removeItem(&enemies[0]);
	scene->removeItem(&enemies[1]);

	player.reset();
	enemies[0].reset();
	enemies[1].reset();

	drawScene();
	drawUI();
}

//fullscreen function
void MainWindow::fullScreen() {
	if (this->isFullScreen()) {
		// Set the background brush to the desired color or image
		scene->setBackgroundBrush(QBrush(QImage(":/ui/sky")));
		this->showNormal();
		//center the settings window
		int x = ((screenGeometry.width() - this->width()) / 2);
		int y = ((screenGeometry.height() - this->height()) / 2);
		settingsWindow->move(x, y);
	}
	else {
		// Set the background brush to the desired color or image
		scene->setBackgroundBrush(QBrush(QImage(":/ui/sky")));
		this->showFullScreen();
	}
}

void MainWindow::win() {
	//Create the window have it be semi transparent
	QDialog* winwindow = new QDialog(this);
	winwindow->setWindowFlags(Qt::FramelessWindowHint);
	winwindow->setStyleSheet("background-color: rgba(0, 0, 0, 200);");
	//Place it in the middle of its parent
	winwindow->setGeometry((this->width() / 2) - 200, (this->height() / 2) - 100, 400, 200);
	winwindow->setFixedSize(400, 200);

	//Create the layout
	QVBoxLayout* layout = new QVBoxLayout;
	winwindow->setLayout(layout);

	//Create the label
	QLabel* winLabel = new QLabel("Success !");
	winLabel->setStyleSheet("color: #FFFFFF; font-size: 30px;");
	layout->addWidget(winLabel, 0, Qt::AlignCenter);

	//Create the restart button
	QPushButton* restartButton = new QPushButton("Restart");
	restartButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
	layout->addWidget(restartButton, 0, Qt::AlignCenter);
	connect(restartButton, SIGNAL(clicked()), winwindow, SLOT(close()));
	connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));

	//Create the quit button
	QPushButton* quitButton = new QPushButton("Quit");
	quitButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
	layout->addWidget(quitButton, 0, Qt::AlignCenter);
	connect(quitButton, SIGNAL(clicked()), winwindow, SLOT(close()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

	//Show the window
	winwindow->show();
}

//GameOver function opens a new window that has two buttons (restart and quit)
void MainWindow::gameOver() {
	if (settingsOpen) {
		// Close the settings screen
		settingsOpen = false;
		settingsWindow->close();
	}
	//Create the window have it be semi transparent
	QDialog* gameOverWindow = new QDialog(this);
	gameOverWindow->setWindowFlags(Qt::FramelessWindowHint);
	gameOverWindow->setStyleSheet("background-color: rgba(0, 0, 0, 200);");
	//Place it in the middle of its parent
	gameOverWindow->setGeometry((this->width() / 2) - 200, (this->height() / 2) - 100, 400, 200);
	gameOverWindow->setFixedSize(400, 200);

	//Create the layout
	QVBoxLayout* layout = new QVBoxLayout;
	gameOverWindow->setLayout(layout);

	//Create the label
	QLabel* gameOverLabel = new QLabel("Game Over");
	gameOverLabel->setStyleSheet("color: #FFFFFF; font-size: 30px;");
	layout->addWidget(gameOverLabel, 0, Qt::AlignCenter);

	//Create the restart button
	QPushButton* restartButton = new QPushButton("Restart");
	restartButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
	layout->addWidget(restartButton, 0, Qt::AlignCenter);
	connect(restartButton, SIGNAL(clicked()), gameOverWindow, SLOT(close()));
	connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));

	//Create the quit button
	QPushButton* quitButton = new QPushButton("Quit");
	quitButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
	layout->addWidget(quitButton, 0, Qt::AlignCenter);
	connect(quitButton, SIGNAL(clicked()), gameOverWindow, SLOT(close()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

	//Show the window
	gameOverWindow->show();
}

//Settings function
void MainWindow::settings() {
	if (!settingsOpen) {
		settingsOpen = true;
		settingsWindow = new QDialog(this);
		settingsWindow->setWindowModality(Qt::ApplicationModal);
		settingsWindow->setWindowFlags(Qt::FramelessWindowHint);
		settingsWindow->setWindowOpacity(0.8);
		setStyleSheet("background-color: rgba(0, 0, 0, 200);");
		settingsWindow->setGeometry((this->width() / 2) - 200, (this->height() / 2) - 100, 400, 200);
		settingsWindow->setFixedSize(400, 400);

		//Create the layout
		QVBoxLayout* layout = new QVBoxLayout;
		layout->setSpacing(10);
		settingsWindow->setLayout(layout);

		//Make the window closable button is round
		QPushButton* closeButton = new QPushButton;
		closeButton->setStyleSheet("background-color: #000000; border-radius: 15px; border: 1px solid #FFFFFF;");
		closeButton->setFixedSize(30, 30);
		closeButton->setIcon(QIcon(":/ui/closeButton"));
		closeButton->setIconSize(QSize(20, 20));
		layout->addWidget(closeButton, 0, Qt::AlignRight);
		connect(closeButton, SIGNAL(clicked()), settingsWindow, SLOT(close()));

		//Create the label
		QLabel* settingsLabel = new QLabel("Settings");
		settingsLabel->setStyleSheet("color: #FFFFFF; font-size: 30px;");
		layout->addWidget(settingsLabel, 0, Qt::AlignCenter);

		//Volume slider
		QSlider* volumeSlider = new QSlider(Qt::Horizontal);
		volumeSlider->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
		volumeSlider->setRange(0, 100);
		volumeSlider->setTickInterval(1);
		volumeSlider->setSingleStep(1);
		volumeSlider->setPageStep(10);
		volumeSlider->setValue(100);
		layout->addWidget(volumeSlider, 0, Qt::AlignCenter);
		//Connect the slider to the changeVolume function in soundManager class pass the value of the slider to the function
		connect(volumeSlider, SIGNAL(valueChanged(int)), &music, SLOT(changeVolume(int)));

		//Label to the right of the slider showing what volume its at
		QLabel* volumeLabel = new QLabel("100%");
		volumeLabel->setStyleSheet("color: #FFFFFF; font-size: 20px;");
		layout->addWidget(volumeLabel, 0, Qt::AlignCenter);
		//Connect the slider to the label pass the value of the slider to the label using a lambda
		connect(volumeSlider, &QSlider::valueChanged, this, [volumeLabel](int value) {
			volumeLabel->setText(QString::number(value) + "%");
			});

		//add mute button to the left of the slider
		QPushButton* muteButton = new QPushButton;
		muteButton->setStyleSheet("background-color: #000000; border-radius: 15px; border: 1px solid #FFFFFF;");
		muteButton->setFixedSize(30, 30);
		muteButton->setIcon(QIcon(":/ui/muteButton"));
		muteButton->setIconSize(QSize(20, 20));
		layout->addWidget(muteButton, 0, Qt::AlignLeft);
		//if the mute button is clicked change the icon to unmute and change the volume to 100
		connect(muteButton, &QPushButton::clicked, this, [muteButton, volumeSlider]() {
			// Check the current icon and set the new icon accordingly
			if (muteButton->icon().cacheKey() == QIcon(":/ui/muteButton").cacheKey()) {
				muteButton->setIcon(QIcon(":/ui/unmuteButton"));
				volumeSlider->setValue(100);
			}
			else {
				muteButton->setIcon(QIcon(":/ui/muteButton"));
				volumeSlider->setValue(0);
			}
			});

		//Fullscreen button
		QPushButton* fullscreenButton = new QPushButton("Fullscreen");
		fullscreenButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
		layout->addWidget(fullscreenButton, 0, Qt::AlignCenter);

		//Connect the fullscreen button to the fullscreen function
		connect(fullscreenButton, SIGNAL(clicked()), this, SLOT(fullScreen()));

		QPushButton* restartButton = new QPushButton("Restart");
		restartButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
		layout->addWidget(restartButton, 0, Qt::AlignCenter);
		connect(restartButton, SIGNAL(clicked()), settingsWindow, SLOT(close()));
		connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));

		//Quit and restart buttons
		QPushButton* quitButton = new QPushButton("Quit");
		quitButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
		layout->addWidget(quitButton, 0, Qt::AlignCenter);
		connect(quitButton, SIGNAL(clicked()), settingsWindow, SLOT(close()));
		connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
		//Connection lambda that turns the settingsOpen bool to false when the window is closed
		connect(settingsWindow, &QDialog::finished, this, []() {
			settingsOpen = false;
			});

		//Add a place to choose the difficulty
		QComboBox* difficultyBox = new QComboBox;
		difficultyBox->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
		difficultyBox->addItem("Easy");
		difficultyBox->addItem("Medium");

		layout->addWidget(difficultyBox, 0, Qt::AlignCenter);
		//Connect the difficulty box to changing the diffculty value using a lambda
		connect(difficultyBox, &QComboBox::currentTextChanged, this, [difficultyBox](QString text) {
			if (text == "Easy") {
				difficulty = 0;
			}
			else if (text == "Medium") {
				difficulty = 1;
			}
			});
		//Show the window
		settingsWindow->exec();
	}
}

void MainWindow::pathFinding(Enemy& enemy) {
	//Start node is enemy spawn
	Node* start = graph->getNode(enemy.x() / 32, enemy.y() / 32);
	//End node is player
	Node* end = graph->getNode(player.x() / 32, player.y() / 32);
	QVector<Node*> path;
	//Add a difficulty check that determines which findPath function is used
	if (difficulty == 0)
		path = graph->findPath2(start, end); //Easy
	else if (difficulty == 1)
		path = graph->findPath(start, end); //Medium

	//Move the enmy to the next node in the path
	if (path.size() > 1 && enemy.getHealth() > 0) {
		enemy.setPos(path[1]->x * 32, path[1]->y * 32);
		enemyCollisionHandler();
	}
}
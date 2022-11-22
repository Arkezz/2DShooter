#include "mainwindow.h"

//Pixel size of each Tile in the grid
const int tileLen = 32;
//Data structure that stores the location of each tile in the grid
//0 = empty, 1 = wall, 2 = player, 3 = enemy
int grid[15][20];

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//Create the scene object and set the scene to the view
	createScene();
	setCentralWidget(view);
	setWindowTitle("2D Shooter");
	music.playSound("bgm");

	//Connections:
	connect(&player, SIGNAL(drawUi()), this, SLOT(drawUI()));
	connect(&player, SIGNAL(openSettings()), this, SLOT(settings()));
	connect(this, SIGNAL(enemyAttack()), &enemies[0], SLOT(attackHandler()));
	connect(&player, SIGNAL(collisionHandler()), this, SLOT(collisionHandler()));
	connect(&player, SIGNAL(drawFootsteps()), this, SLOT(drawFootsteps()));

	drawScene();
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
	scene->addItem(newItem);
}

void MainWindow::drawScene() {
	scene->clear();
	//Read the tmx file then go line by line in the csv
	QFile file(":/maps/map1");
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

	// Draw the scene according to the grid
	for (int y = 0; y < 15; y++) {
		for (int x = 0; x < 20; x++) {
			if (grid[y][x] == 1) {
				addTile(x, y, "wall");
			}
			else if (grid[y][x] == 0) {
				addTile(x, y, "grass");
			}
		}
	}
	//Add player to the middle of the scene
	player.setPos(400, 200);
	player.setFocus();
	scene->addItem(&player);
	//Add enemy to the middle of the scene
	enemies[0].setPos(400, 80);
	scene->addItem(&enemies[0]);

	for (int i = 0; i < player.getHealth(); i++) {
		QGraphicsPixmapItem* heart = new QGraphicsPixmapItem;
		heart->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
		heart->setPos(0 + (i * tileLen), 0);
		scene->addItem(heart);
		hearts.push_back(heart);
	}
	object = new Collectibles(Collectibles::bullet);
	object->setPos(400, 400);
	scene->addItem(object);
	object->animHandler();
}

//Function that controls the ui
void MainWindow::drawUI() {
	//Check the health of the player and turn each heart empty if the player is hurt start from the end of the vector
	for (int i = player.getHealth(); i < hearts.size(); i++) {
		hearts[i]->setPixmap(QPixmap(":/ui/emptyHeart").scaled(tileLen, tileLen));
		//remove it from vector
		hearts.erase(hearts.begin() + i);
	}

	//If the player gained a heart add a new heart to the vector
	if (player.getHealth() > hearts.size()) {
		QGraphicsPixmapItem* heart = new QGraphicsPixmapItem;
		heart->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
		heart->setPos(0 + (hearts.size() * tileLen), 0);
		scene->addItem(heart);
		hearts.push_back(heart);
	} //Check if the player has any empty hearts and remove them from the vector

	if (player.getHealth() == 0) {
		scene->removeItem(&player);
		gameOver();
	}

	if (enemies[0].getHealth() == 0) {
		//Timer connected to enemy deathHandler
		QTimer* deathTimer = new QTimer;
		deathTimer->start(300);
		connect(deathTimer, SIGNAL(timeout()), &enemies[0], SLOT(deathHandler()));
		//wait 2 seconds then remove the enemy from the scene lambda
		QTimer::singleShot(2000, [this]() {
			scene->removeItem(&enemies[0]);
			});
	}

	//Show the amount of ammo the player next to the hearts
	QGraphicsTextItem* ammo = new QGraphicsTextItem;
	ammo->setPlainText("Ammo: " + QString::number(player.getAmmo()) + "/4");
	ammo->setPos(0, tileLen);
	scene->addItem(ammo);

	//Text that shows the player's status if hes on a powerup or not
	QGraphicsTextItem* statusText = new QGraphicsTextItem;

	statusText->setPlainText("Normal");

	//if (player.getStatus() == 1) {
	//	statusText->setPlainText("Speed");
	//}
	//else if (player.getStatus() == 2) {
	//	statusText->setPlainText("Invincible");
	//}

	statusText->setPos(0, tileLen * 2);
	scene->addItem(statusText);
}

void MainWindow::drawFootsteps() {
	//draw a round circle to simulate footsteps on the grass once there are 5 footsteps delete the oldest one
	if (footsteps.size() < 5) {
		QGraphicsEllipseItem* footstep = new QGraphicsEllipseItem;
		footstep->setRect(player.x(), player.y(), 5, 5);
		footstep->setBrush(QBrush(QColor(0, 0, 0, 100)));
		scene->addItem(footstep);
		footsteps.push_back(footstep);
	}
	else {
		scene->removeItem(footsteps[0]);
		footsteps.erase(footsteps.begin());
		QGraphicsEllipseItem* footstep = new QGraphicsEllipseItem;
		footstep->setRect(player.x() + 10, player.y() + 10, 10, 10);
		footstep->setBrush(QBrush(QColor(0, 0, 0, 100)));
		scene->addItem(footstep);
		footsteps.push_back(footstep);
	}
}

//collisionHandler
void MainWindow::collisionHandler() {
	//Check if the player is colliding with any enemy
	QList<QGraphicsItem*> colliding_items = player.collidingItems();
	for (int i = 0; i < colliding_items.size(); i++) {
		if (typeid(*(colliding_items[i])) == typeid(Enemy)) {
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
		}//if its colliding with a collectible, remove it from the scene and decide its type
		else if (typeid(*(colliding_items[i])) == typeid(Collectibles)) {
			scene->removeItem(colliding_items[i]);
			//If its a bullet increase the players ammo
			if (dynamic_cast<Collectibles*>(colliding_items[i])->getType() == 0) {
				player.setHealth(player.getHealth() + 1);
				drawUI();
				player.pickUp();
                //enemies[0].setPixmap(QPixmap(":/enemy1/hurt").transformed(QTransform().scale(-1, 1)).scaled(64,64));
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
	//Reset the player
	player.setPos(400, 200);
	player.setAmmo(0);
	player.setHealth(3);
	player.setFocus();
	scene->addItem(&player);
	//Reset the enemy
	enemies[0].setPos(400, 80);
	scene->addItem(&enemies[0]);
	//Reset the hearts
	for (int i = 0; i < hearts.size(); i++) {
		hearts[i]->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
	}
	//Reset the object
	scene->addItem(object);
}

//fullscreen function
void MainWindow::fullScreen() {
	if (this->isFullScreen()) {
		this->showNormal();
	}
	else {
		this->showFullScreen();
		//Fill the black parts the screen with the background
	}
}

//GameOver function opens a new window that has two buttons (restart and quit)
void MainWindow::gameOver() {
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
	if (!settingsWindow) {
		QDialog* settingsWindow = new QDialog(this);
		settingsWindow->setWindowModality(Qt::ApplicationModal);
		settingsWindow->setWindowFlags(Qt::FramelessWindowHint);
		settingsWindow->setWindowOpacity(0.8);
		setStyleSheet("background-color: rgba(0, 0, 0, 200);");
		settingsWindow->setGeometry((this->width() / 2) - 200, (this->height() / 2) - 100, 400, 200);
		settingsWindow->setFixedSize(400, 400);

		//Create the layout
		QVBoxLayout* layout = new QVBoxLayout;
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
		connect(volumeSlider, &QSlider::valueChanged, [volumeLabel](int value) {
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
		connect(muteButton, &QPushButton::clicked, [muteButton, volumeSlider]() {
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
		//Show the window
		settingsWindow->exec();
	}
}

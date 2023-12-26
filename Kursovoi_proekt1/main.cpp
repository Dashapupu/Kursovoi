#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QDir>
#include <QScrollArea>
#include <QLabel>
#include <QDialog>
#include <QHBoxLayout>

class MedicalImageViewer : public QWidget {
    Q_OBJECT

public:
    MedicalImageViewer(const QString &folderPath, QWidget *parent = nullptr)
        : QWidget(parent), imagesPerRow(1), folderPath(folderPath) {
        setupUI();
        loadImages();
    }

private slots:
    void openImage(const QString &imagePath) {
        QPixmap image(imagePath);
        if (image.isNull()) {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не удается загрузить файл: %1").arg(imagePath));
        } else {
            // Открываем изображение в отдельном окне
            QDialog *imageDialog = new QDialog(this);
            imageDialog->setWindowTitle(imagePath);
            imageDialog->setFixedSize(image.size());

            QLabel *imageLabel = new QLabel;
            imageLabel->setPixmap(image);

            QHBoxLayout *layout = new QHBoxLayout(imageDialog);
            layout->addWidget(imageLabel);

            imageDialog->exec();
        }
    }

    void changeGridLayout() {
        clearGridLayout();
        imagesPerRow = (imagesPerRow % 4) + 1;  // Переключаем количество изображений в строке от 1 до 4
              loadImages();
              adjustSize();
          }

      private:
          int imagesPerRow;

          void setupUI() {
              QVBoxLayout *mainLayout = new QVBoxLayout;
              scrollArea = new QScrollArea;
              scrollArea->setWidgetResizable(true);

              centralLayout = new QVBoxLayout;
              centralLayout->setAlignment(Qt::AlignTop);

              scrollArea->setLayout(centralLayout);
              mainLayout->addWidget(scrollArea);
              setLayout(mainLayout);

              // Устанавливаем минимальные размеры окна
              setMinimumWidth(720);
              setMinimumHeight(imagesPerRow * 780);

              // Используем QGridLayout
              gridLayout = new QGridLayout;
              centralLayout->addLayout(gridLayout);

              // Кнопка для смены формата отображения
              QPushButton *changeFormatButton = new QPushButton(tr("Формат отображения"));
              connect(changeFormatButton, &QPushButton::clicked, this,
      &MedicalImageViewer::changeGridLayout);
              mainLayout->addWidget(changeFormatButton);
          }

          void loadImages() {
              QDir directory(folderPath);
              QStringList imageFilters;
              imageFilters << "*.png" << "*.jpg" << "*.bmp";
              QStringList imageFiles =
                  directory.entryList(imageFilters, QDir::Files | QDir::NoDotAndDotDot);

              int imageCount = 0;

              for (const QString &fileName : imageFiles) {
                  if (imageCount >= 6) {
                      break;  // Ограничиваем до максимум 6 изображений
                              }

                              QPushButton *imageButton = new QPushButton;
                              QPixmap preview = createPreview(directory.filePath(fileName));

                              if (preview.isNull()) {
                                  QMessageBox::warning(this, tr("Ошибка"),
                                                       tr("Не удается загрузить файл: %1").arg(directory.filePath(fileName)));
                                  continue;
                              }

                              imageButton->setIcon(QIcon(preview));
                              imageButton->setIconSize(QSize(100, 100));  // Размер иконки
                              imageButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                              // Захватываем переменную directory в лямбда-выражении
                              connect(imageButton, &QPushButton::clicked, [this, fileName, directory]() {
                                  openImage(directory.filePath(fileName));
                              });

                              // Делаем изображение и его бокс квадратными
                              imageButton->setFixedSize(120, 120);
                              imageButton->setStyleSheet("QPushButton { border: 1px solid black; }");

                              // Добавляем кнопку в QGridLayout
                              int row = imageCount / imagesPerRow;
                              int col = imageCount % imagesPerRow;
                              gridLayout->addWidget(imageButton, row, col);

                              ++imageCount;
                          }
                      }

                      void clearGridLayout() {
                          QLayoutItem *child;
                          while ((child = gridLayout->takeAt(0)) != nullptr) {
                              delete child->widget();
                              delete child;
                          }
                      }

                      QPixmap createPreview(const QString &imagePath) {
                          QPixmap original(imagePath);
                          return original.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                      }
                      QVBoxLayout *centralLayout;
                        QScrollArea *scrollArea;
                        QGridLayout *gridLayout;
                        QString folderPath;
                    };

                    int main(int argc, char *argv[]) {
                        QApplication app(argc, argv);

                        QString initialPath ="D:/Med";

                        QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Выберите папку с изображениями", initialPath);

                        if (folderPath.isEmpty()) {
                            return 0;
                        }

                        MedicalImageViewer viewer(folderPath);
                        viewer.show();

                        return app.exec();
                    }

 #include "main.moc"

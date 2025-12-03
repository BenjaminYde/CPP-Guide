#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QListWidget>
#include <QFileDialog>
#include <QColorDialog>
#include <QPainter>
#include <QImage>
#include <QFileInfo>
#include <QMessageBox>
#include <QSlider>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QStyleFactory>
#include <QDir>

class MainWindow : public QMainWindow {

public:
    MainWindow() {
        setWindowTitle("Image Blender");
        resize(600, 850); 

        auto *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        auto *layout = new QVBoxLayout(centralWidget);

        // --- 1. Top Section: Load Images ---
        auto *btnLoad = new QPushButton("1. Select Images", this);
        layout->addWidget(btnLoad);

        fileListWidget = new QListWidget(this);
        fileListWidget->setFixedHeight(100);
        layout->addWidget(fileListWidget);

        // --- 2. Settings Section ---
        auto *settingsGroup = new QGroupBox("Tint Settings", this);
        auto *settingsLayout = new QVBoxLayout(settingsGroup);

        // Row 1: Color Picker
        auto *colorRow = new QHBoxLayout();
        auto *btnPickColor = new QPushButton("Pick Color", this);
        colorPreviewLabel = new QLabel(this);
        colorPreviewLabel->setFixedSize(40, 25);
        selectedColor = Qt::yellow;

        colorRow->addWidget(btnPickColor);
        colorRow->addWidget(colorPreviewLabel);
        colorRow->addStretch();
        settingsLayout->addLayout(colorRow);

        // Row 2: Blend Mode Dropdown
        auto *modeRow = new QHBoxLayout();
        auto *lblMode = new QLabel("Blend Mode:", this);
        blendModeCombo = new QComboBox(this);
        
        blendModeCombo->addItem("Tint (Source Atop) - Default", QPainter::CompositionMode_SourceAtop);
        blendModeCombo->addItem("Normal (Source Over)", QPainter::CompositionMode_SourceOver);
        blendModeCombo->addItem("Multiply", QPainter::CompositionMode_Multiply);
        blendModeCombo->addItem("Screen", QPainter::CompositionMode_Screen);
        blendModeCombo->addItem("Overlay", QPainter::CompositionMode_Overlay);
        blendModeCombo->addItem("Darken", QPainter::CompositionMode_Darken);
        blendModeCombo->addItem("Lighten", QPainter::CompositionMode_Lighten);
        blendModeCombo->addItem("Color Burn", QPainter::CompositionMode_ColorBurn);
        blendModeCombo->addItem("Color Dodge", QPainter::CompositionMode_ColorDodge);
        blendModeCombo->addItem("Hard Light", QPainter::CompositionMode_HardLight);
        blendModeCombo->addItem("Soft Light", QPainter::CompositionMode_SoftLight);
        blendModeCombo->addItem("Difference", QPainter::CompositionMode_Difference);
        
        modeRow->addWidget(lblMode);
        modeRow->addWidget(blendModeCombo);
        settingsLayout->addLayout(modeRow);

        // Row 3: Opacity Slider
        auto *sliderRow = new QHBoxLayout();
        auto *lblOpacity = new QLabel("Blend Hardness (Opacity):", this);
        opacitySlider = new QSlider(Qt::Horizontal, this);
        opacitySlider->setRange(0, 255);
        opacitySlider->setValue(100);

        // NEW: Label to show 0.0 - 1.0 value
        opacityValueLabel = new QLabel("0.39", this);
        opacityValueLabel->setFixedWidth(35); 
        opacityValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        sliderRow->addWidget(lblOpacity);
        sliderRow->addWidget(opacitySlider);
        sliderRow->addWidget(opacityValueLabel);
        settingsLayout->addLayout(sliderRow);

        layout->addWidget(settingsGroup);

        // --- 3. Output Configuration Section  ---
        auto *outputGroup = new QGroupBox("Output Configuration", this);
        auto *outputLayout = new QVBoxLayout(outputGroup);

        // Output Directory Row
        auto *dirRow = new QHBoxLayout();
        auto *lblDir = new QLabel("Output Folder:", this);
        outputDirEdit = new QLineEdit("[Source Folder]/processed", this);
        outputDirEdit->setReadOnly(true);
        auto *btnBrowseDir = new QPushButton("Select Folder...", this);

        dirRow->addWidget(lblDir);
        dirRow->addWidget(outputDirEdit);
        dirRow->addWidget(btnBrowseDir);
        outputLayout->addLayout(dirRow);

        // Suffix Row (Modified)
        auto *suffixRow = new QHBoxLayout();
        suffixCheckBox = new QCheckBox("Append Suffix:", this);
        suffixCheckBox->setChecked(false);
        
        suffixEdit = new QLineEdit("_processed", this);
        suffixEdit->setEnabled(false); 
        // Initial visual state for disabled box
        suffixEdit->setStyleSheet("color: gray; background-color: #252525; border: 1px solid #444;");
        
        suffixRow->addWidget(suffixCheckBox);
        suffixRow->addWidget(suffixEdit);
        outputLayout->addLayout(suffixRow);

        layout->addWidget(outputGroup);

        // --- 4. Preview Section ---
        auto *previewGroup = new QGroupBox("Preview (First Image)", this);
        auto *previewLayout = new QVBoxLayout(previewGroup);
        imagePreviewLabel = new QLabel("No image selected", this);
        imagePreviewLabel->setAlignment(Qt::AlignCenter);
        imagePreviewLabel->setMinimumHeight(200);
        previewLayout->addWidget(imagePreviewLabel);
        
        layout->addWidget(previewGroup);

        // --- 5. Export Button ---
        auto *btnExport = new QPushButton("Export All Files", this);
        btnExport->setMinimumHeight(40);
        layout->addWidget(btnExport);

        // --- Connections ---
        connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadFiles);
        connect(btnPickColor, &QPushButton::clicked, this, &MainWindow::pickColor);
        connect(btnExport, &QPushButton::clicked, this, &MainWindow::processAndExport);
        connect(btnBrowseDir, &QPushButton::clicked, this, &MainWindow::selectOutputDirectory);
        
        // Settings updates
        connect(opacitySlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
        connect(fileListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updatePreview);
        connect(blendModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
                this, &MainWindow::updatePreview);

        // Update the 0-1 label when slider changes
        connect(opacitySlider, &QSlider::valueChanged, this, [this](int value){
            double normalized = value / 255.0;
            opacityValueLabel->setText(QString::number(normalized, 'f', 2));
        });

        // Toggle Suffix Edit based on Checkbox with visual greying out
        connect(suffixCheckBox, &QCheckBox::toggled, this, [this](bool checked){
            suffixEdit->setEnabled(checked);
            if (checked) {
                suffixEdit->setStyleSheet(""); // Reset to default theme style
            } else {
                suffixEdit->setStyleSheet("color: gray; background-color: #252525; border: 1px solid #444;");
            }
        });

        updateColorPreview();
    }

private:
    QListWidget *fileListWidget;
    QStringList selectedFiles;
    QLabel *colorPreviewLabel;
    QColor selectedColor;
    
    QSlider *opacitySlider;
    QLabel *opacityValueLabel;
    QComboBox *blendModeCombo;
    
    // Output UI
    QLineEdit *outputDirEdit;
    QString m_customOutputDir;
    QCheckBox *suffixCheckBox;
    QLineEdit *suffixEdit;
    
    QLabel *imagePreviewLabel;

    QImage m_cachedImage;
    QString m_cachedPath;

    void updateColorPreview() {
        QString style = QString("background-color: %1; border: 1px solid #555;").arg(selectedColor.name());
        colorPreviewLabel->setStyleSheet(style);
        updatePreview();
    }

    void pickColor() {
        QColor color = QColorDialog::getColor(selectedColor, this, "Select Tint Color");
        if (color.isValid()) {
            selectedColor = color;
            updateColorPreview();
        }
    }

    void selectOutputDirectory() {
        QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory", 
                                                        "", QFileDialog::ShowDirsOnly);
        if (!dir.isEmpty()) {
            m_customOutputDir = dir;
            outputDirEdit->setText(dir);
        }
    }

    void loadFiles() {
        QStringList files = QFileDialog::getOpenFileNames(this, 
            "Select Images", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (files.isEmpty()) return;

        selectedFiles = files;
        fileListWidget->clear();
        for (const QString &file : selectedFiles) {
            fileListWidget->addItem(file);
        }
        
        if (fileListWidget->count() > 0) {
            fileListWidget->setCurrentRow(0);
        }
    }

    void applyTintToImage(QImage &image) {
        if (image.isNull()) return;

        if (image.format() != QImage::Format_ARGB32_Premultiplied) {
            image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        }

        QPainter painter(&image);
        QColor overlayColor = selectedColor;
        overlayColor.setAlpha(opacitySlider->value());

        auto mode = static_cast<QPainter::CompositionMode>(blendModeCombo->currentData().toInt());
        painter.setCompositionMode(mode);
        
        painter.fillRect(image.rect(), overlayColor);
        painter.end();
    }

    void updatePreview() {
        if (selectedFiles.isEmpty()) {
            imagePreviewLabel->setText("No images loaded");
            return;
        }

        QString previewFile;
        QListWidgetItem *item = fileListWidget->currentItem();
        if (item) {
            previewFile = item->text();
        } else {
            previewFile = selectedFiles.first();
        }

        if (previewFile != m_cachedPath || m_cachedImage.isNull()) {
            m_cachedImage.load(previewFile);
            if (!m_cachedImage.isNull() && m_cachedImage.format() != QImage::Format_ARGB32_Premultiplied) {
                m_cachedImage = m_cachedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            }
            m_cachedPath = previewFile;
        }

        if (m_cachedImage.isNull()) return;

        QImage tempImg = m_cachedImage; 
        applyTintToImage(tempImg);

        QPixmap px = QPixmap::fromImage(tempImg);
        imagePreviewLabel->setPixmap(px.scaled(imagePreviewLabel->size(), 
                                        Qt::KeepAspectRatio, 
                                        Qt::SmoothTransformation));
    }

    void processAndExport() {
        if (selectedFiles.isEmpty()) {
            QMessageBox::warning(this, "No Files", "Please select images first.");
            return;
        }

        // 1. Determine Suffix
        QString suffix = "";
        if (suffixCheckBox->isChecked()) {
            suffix = suffixEdit->text();
        }

        int count = 0;
        int skippedOrRenamed = 0;

        for (const QString &filePath : selectedFiles) {
            QImage finalImage(filePath);
            if (finalImage.isNull()) continue;

            applyTintToImage(finalImage);

            QFileInfo sourceInfo(filePath);
            
            // 2. Determine Output Directory
            QString finalOutputDir;
            if (m_customOutputDir.isEmpty()) {
                // Default: Create "processed" folder in source directory
                finalOutputDir = sourceInfo.absolutePath() + "/processed";
            } else {
                finalOutputDir = m_customOutputDir;
            }

            // Ensure directory exists
            QDir dir(finalOutputDir);
            if (!dir.exists()) {
                dir.mkpath(".");
            }

            // 3. Construct Filename
            QString newName = sourceInfo.baseName() + suffix + "." + sourceInfo.suffix();
            QString targetPath = dir.filePath(newName);

            // 4. Overwrite Protection
            // QFileInfo comparison handles standard path differences (e.g. / vs \)
            if (QFileInfo(targetPath) == QFileInfo(filePath)) {
                // Collision detected (Source == Destination)
                // We enforce a safe suffix
                newName = sourceInfo.baseName() + suffix + "_copy." + sourceInfo.suffix();
                targetPath = dir.filePath(newName);
                skippedOrRenamed++;
            }

            if (finalImage.save(targetPath)) {
                count++;
            }
        }

        QString msg = QString("Exported %1 images.").arg(count);
        if (skippedOrRenamed > 0) {
            msg += QString("\n\nNote: %1 files were renamed with '_copy' to avoid overwriting originals.").arg(skippedOrRenamed);
        }

        QMessageBox::information(this, "Done", msg);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);

    MainWindow window;
    window.show();

    return app.exec();
}
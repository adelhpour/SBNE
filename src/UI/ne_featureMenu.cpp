#include "ne_featureMenu.h"

// MyLabel

MyLabel::MyLabel(const QString& text, QWidget* parent) : QLabel(parent) {
    setContentsMargins(0, 0, 0, 0);
    QFont font;
    font.setBold(true);
    font.setCapitalization(QFont::Capitalize);
    font.setPointSize(12);
    setFont(font);
    
    if (!text.isEmpty())
        setText(text);
}

// MyLineEdit

MyLineEdit::MyLineEdit(QWidget* parent) : QLineEdit(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QLineEdit {border: 1px solid LightSlateGray; border-radius: 10px; padding: 0 8px; background: GhostWhite; selection-background-color: LightGray;} QLineEdit::read-only {color: gray}");
    setFixedWidth(175);
    setFixedHeight(22.0);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
}

void MyLineEdit::setText(const QString& text) {
    QLineEdit::setText(text);
    setToolTip(text);
    setCursorPosition(0);
}

void MyLineEdit::insert(const QString& text) {
    QLineEdit::insert(text);
    setToolTip(text);
    setCursorPosition(0);
}

// MySpinBox

MySpinBox::MySpinBox(QWidget* parent) : QSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setSpecialValueText(tr("N/A"));
    setStyleSheet("QSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QSpinBox::up-button { background-color: transparent; }" "QSpinBox::down-button { background-color: transparent;}" "QSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
}

// MyDoubleSpinBox

MyDoubleSpinBox::MyDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setDecimals(1);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QDoubleSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QDoubleSpinBox::up-button { background-color: transparent; }" "QDoubleSpinBox::down-button { background-color: transparent;}" "QDoubleSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QDoubleSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
}

// MyDualSpinBox

MyDualSpinBox::MyDualSpinBox(const QString& type) {
    setContentsMargins(0, 0, 0, 0);
    setLayout(&contentLayout);
    setType(type);
    setFixedWidth(220.0);
    
    connect(&firstSpinBox, &QDoubleSpinBox::editingFinished, this, [this] () {
        emit this->valueChanged(this->firstSpinBox.value(), this->secondSpinBox.value()); });
    connect(&secondSpinBox, &QDoubleSpinBox::editingFinished, this, [this] () { emit this->valueChanged(this->firstSpinBox.value(), this->secondSpinBox.value()); });
    
    contentLayout.setSpacing(0);
    contentLayout.addWidget(&firstLabel, 0, 0);
    contentLayout.addItem(new QSpacerItem(7, 0), 0, 1);
    contentLayout.addWidget(&firstSpinBox, 0, 2);
    contentLayout.addItem(new QSpacerItem(15, 0), 0, 3);
    contentLayout.addWidget(&secondLabel, 0, 4);
    contentLayout.addItem(new QSpacerItem(7, 0), 0, 5);
    contentLayout.addWidget(&secondSpinBox, 0, 6);
    
    resetValues();
}

const double MyDualSpinBox::first() const {
    return firstSpinBox.value();
}

const double MyDualSpinBox::second() const {
    return secondSpinBox.value();
}

void MyDualSpinBox::setType(const QString& type) {
    _type = type;
    if (_type == "Position") {
        firstLabel.setText("X");
        secondLabel.setText("Y");
    }
    else if (_type == "Dimensions") {
        firstLabel.setText("W");
        secondLabel.setText("H");
    }
}

void MyDualSpinBox::setCurrentValue(const double& first, const double& second) {
    firstSpinBox.setValue(first);
    firstSpinBox.blockSignals(false);
    secondSpinBox.setValue(second);
    secondSpinBox.blockSignals(false);
}

void MyDualSpinBox::setRange(MainWindow* mw) {
    if (mw) {
        if (_type == "Position") {
            setFirstRange(mw->getScene()->sceneRect().x(), mw->getScene()->sceneRect().x() + mw->getScene()->sceneRect().width());
            setSecondRange(mw->getScene()->sceneRect().y(), mw->getScene()->sceneRect().y() + mw->getScene()->sceneRect().height());
        }
        else if (_type == "Dimensions") {
            setFirstRange(1.0, mw->getScene()->sceneRect().width());
            setSecondRange(1.0, mw->getScene()->sceneRect().height());
        }
    }
    
    resetValues();
}

void MyDualSpinBox::setFirstRange(const double& min, const double& max) {
    firstSpinBox.setRange(min, max);
}

void MyDualSpinBox::setSecondRange(const double& min, const double& max) {
    secondSpinBox.setRange(min, max);
}

void MyDualSpinBox::resetValues() {
    firstSpinBox.blockSignals(true);
    firstSpinBox.setValue(firstSpinBox.minimum());
    secondSpinBox.blockSignals(true);
    secondSpinBox.setValue(secondSpinBox.minimum());
}

// MyRelAbsSpinBox

MyRelAbsSpinBox::MyRelAbsSpinBox() {
    setContentsMargins(0, 0, 0, 0);
    setLayout(&contentLayout);
    setFixedHeight(70);
    
    connect(&absoluteValueSpinBox, &QDoubleSpinBox::editingFinished, this, [this] () { emit this->valueChanged(this->absoluteValueSpinBox.value(), this->relativeValueSpinBox.value()); });
    relativeValueSpinBox.setSuffix("%");
    relativeValueSpinBox.setRange(-1, 100);
    connect(&relativeValueSpinBox, &QSpinBox::editingFinished , this, [this] () {
        emit this->valueChanged(this->absoluteValueSpinBox.value(), this->relativeValueSpinBox.value()); });
    
    contentLayout.addWidget(new QLabel("abs"), 0, 0, Qt::AlignLeft);
    absoluteValueSpinBox.setFixedWidth(70);
    contentLayout.addWidget(&absoluteValueSpinBox, 0, 1, Qt::AlignRight);
    contentLayout.addWidget(new QLabel("rel"), 1, 0, Qt::AlignLeft);
    relativeValueSpinBox.setFixedWidth(70);
    contentLayout.addWidget(&relativeValueSpinBox, 1, 1, Qt::AlignRight);
}

const double MyRelAbsSpinBox::absoluteValue() const {
    return absoluteValueSpinBox.value();
}

const int MyRelAbsSpinBox::relativeValue() const {
    return relativeValueSpinBox.value();
}

void MyRelAbsSpinBox::setCurrentValue(const double& abs, const double& rel) {
    absoluteValueSpinBox.setValue(abs);
    absoluteValueSpinBox.blockSignals(false);
    relativeValueSpinBox.setValue(rel);
    relativeValueSpinBox.blockSignals(false);
}

void MyRelAbsSpinBox::setRange(const double& min, const double& max) {
    absoluteValueSpinBox.setRange(min, max);
}

void MyRelAbsSpinBox::resetValues() {
    absoluteValueSpinBox.blockSignals(true);
    absoluteValueSpinBox.setValue(absoluteValueSpinBox.minimum());
    relativeValueSpinBox.blockSignals(true);
    relativeValueSpinBox.setValue(relativeValueSpinBox.minimum());
}

// MyPlainTextWidget

MyPlainTextWidget::MyPlainTextWidget() {
    setContentsMargins(0, 0, 0, 0);
    setLayout(&contentLayout);
    plainTextLineEdit.setFixedWidth(150.0);
    plainTextLineEdit.setFixedHeight(22.0);
    
    connect(&plainTextLineEdit, &QLineEdit::returnPressed, this, [this] () {
        if (plainTextLineEdit.isEnabled())
            emit this->plainTextChanged(this->plainTextLineEdit.text()); });
    
    importTextPushButton.setText("Import from");
    importTextPushButton.setMenu(&importMenu);
    
    contentLayout.addWidget(&plainTextLineEdit, 0, 0);
    contentLayout.addWidget(&importTextPushButton, 0, 1);
}

void MyPlainTextWidget::setPlainText(const QString& plainText) {
    plainTextLineEdit.insert(plainText);
}

QString MyPlainTextWidget::plainText() {
    return plainTextLineEdit.text();
}

void MyPlainTextWidget::enable(NGraphicalObject* gO) {
    if (gO) {
        plainTextLineEdit.clear();
        if (ne_ne_isSetName(gO))
            connect(importMenu.addAction("Name"), &QAction::triggered, this, [this, gO] () { this->plainTextLineEdit.setText(ne_ne_getName(gO).c_str());
                //emit this->plainTextChanged(this->plainTextLineEdit.text());
            });
        if (ne_ne_isSetId(gO))
            connect(importMenu.addAction("Id"), &QAction::triggered, this, [this, gO] () { this->plainTextLineEdit.setText(ne_ne_getId(gO).c_str());
                //emit this->plainTextChanged(this->plainTextLineEdit.text());
            });
        if (ne_go_isSetGlyphId(gO))
            connect(importMenu.addAction("Layout Id"), &QAction::triggered, this, [this,gO] () { this->plainTextLineEdit.setText(ne_go_getGlyphId(gO).c_str());
                //emit this->plainTextChanged(this->plainTextLineEdit.text());
            });
        
        plainTextLineEdit.setEnabled(true);
    }
}

void MyPlainTextWidget::resetValues() {
    plainTextLineEdit.setEnabled(false);
    plainTextLineEdit.setText("N/A");
    importMenu.clear();
}

// MyComboBox

MyComboBox::MyComboBox(QWidget* parent) : QComboBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QComboBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; color: black; padding: 1px 18px 1px 3px; }" "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left-width: 1px; border-left-color: LightSlateGray;border-left-style: solid; border-top-right-radius: 5px; border-bottom-right-radius: 5px; }" "QComboBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; } QComboBox::disabled { color: gray;} QComboBox QAbstractItemView { border: 1px solid LightSlateGray; background-color: white; min-width: 100px; }");
    
    setAttribute(Qt::WA_MacShowFocusRect, 0);
}

// MyFontSizeComboBox

MyFontSizeComboBox::MyFontSizeComboBox() {
    setContentsMargins(0, 0, 0, 0);
    setLayout(&contentLayout);
    setFixedHeight(70);
    QStringList fontSizeAbsoluteItems;
    QStringList fontSizeRelativeItems;
    int i = 0;
    while (i <= 72) {
        fontSizeAbsoluteItems << QString::number(i);
        
        if (i == 0)
            i += 5;
        else if (i < 12)
            ++i;
        else if (i < 28)
            i += 2;
        else if (i < 36)
            i += 8;
        else if (i < 48)
            i += 12;
        else
            i += 24;
    }
    
    absoluteValueComboBox.addItems(fontSizeAbsoluteItems);
    connect(&absoluteValueComboBox, QOverload<int>::of(&QComboBox::activated), this, [this] (const int& index) {
        if (index == 0) {
            if (relativeValueComboBox.itemText(0) == "%0")
                relativeValueComboBox.removeItem(0);
            this->relativeValueComboBox.setEnabled(true);
        }
        else {
            if (relativeValueComboBox.itemText(0) != "%0")
                relativeValueComboBox.insertItem(0, "%0");
            this->relativeValueComboBox.setEnabled(false);
        }
        
        this->relativeValueComboBox.setCurrentIndex(0);
    });
    connect(&absoluteValueComboBox, QOverload<int>::of(&QComboBox::activated), this, [this] () { emit this->valueChanged(this->absoluteValueComboBox.currentText(), this->relativeValueComboBox.currentText().split("%").at(1)); });
    
    for (int i = 0; i <= 100; i += 10)
        fontSizeRelativeItems << "%" + QString::number(i);
    
    
    relativeValueComboBox.addItems(fontSizeRelativeItems);
    connect(&relativeValueComboBox, QOverload<int>::of(&QComboBox::activated), this, [this] () { emit this->valueChanged(this->absoluteValueComboBox.currentText(), this->relativeValueComboBox.currentText().split("%").at(1)); });
    
    contentLayout.addWidget(new QLabel("abs"), 0, 0, Qt::AlignLeft);
    absoluteValueComboBox.setFixedWidth(70);
    contentLayout.addWidget(&absoluteValueComboBox, 0, 1, Qt::AlignRight);
    contentLayout.addWidget(new QLabel("rel"), 1, 0, Qt::AlignLeft);
    relativeValueComboBox.setFixedWidth(70);
    contentLayout.addWidget(&relativeValueComboBox, 1, 1, Qt::AlignRight);
}

void MyFontSizeComboBox::setCurrentValue(const double& abs, const double& rel) {
    absoluteValueComboBox.setEnabled(true);
    absoluteValueComboBox.setCurrentText(QString::number(abs));
    
    if (rel > 0.000001) {
        relativeValueComboBox.setEnabled(true);
        relativeValueComboBox.setCurrentText("%" + QString::number(rel));
    }
}

void MyFontSizeComboBox::resetValues() {
    absoluteValueComboBox.setCurrentIndex(0);
    absoluteValueComboBox.setEnabled(false);
    relativeValueComboBox.setCurrentIndex(0);
    relativeValueComboBox.setEnabled(false);
}

// MyDashArrayComboBox

MyDashArrayComboBox::MyDashArrayComboBox(QWidget* parent) : MyComboBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setIconSize(QSize(100, 10));
    setStyleSheet("QComboBox { border: 1px solid LightSlateGray; border-radius: 5px; color: black; padding: 1px 18px 1px 3px; min-width: 6em; }" "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left-width: 1px; border-left-color: LightSlateGray;border-left-style: solid; border-top-right-radius: 5px; border-bottom-right-radius: 5px; }" "QComboBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; } QComboBox::disabled { color: gray;} QComboBox QAbstractItemView { border: 1px solid LightSlateGray; background-color: white; }");
    
    // solid line
    QPixmap solidLinePixmap(QSize(100, 10));
    solidLinePixmap.fill(Qt::transparent);
    QPainter solidLinePaint(&solidLinePixmap);
    solidLinePaint.setPen(QPen(QBrush(Qt::black), 3, Qt::SolidLine));
    solidLinePaint.drawLine(5, 5, 88, 5);
    solidLinePaint.end();
    addItem(solidLinePixmap, "");
    
    // dash line
    QPixmap dashLinePixmap(QSize(100, 10));
    dashLinePixmap.fill(Qt::transparent);
    QPainter dashLinePaint(&dashLinePixmap);
    dashLinePaint.setPen(QPen(QBrush(Qt::black), 3, Qt::DashLine));
    dashLinePaint.drawLine(5, 5, 88, 5);
    dashLinePaint.end();
    addItem(dashLinePixmap, "");
    
    // dot line
    QPixmap dotLinePixmap(QSize(100, 10));
    dotLinePixmap.fill(Qt::transparent);
    QPainter dotLinePaint(&dotLinePixmap);
    dotLinePaint.setPen(QPen(QBrush(Qt::black), 3, Qt::DotLine));
    dotLinePaint.drawLine(5, 5, 88, 5);
    dotLinePaint.end();
    addItem(dotLinePixmap, "");
    
    // dash dot line
    QPixmap dashDotLinePixmap(QSize(100, 10));
    dashDotLinePixmap.fill(Qt::transparent);
    QPainter dashDotLinePaint(&dashDotLinePixmap);
    dashDotLinePaint.setPen(QPen(QBrush(Qt::black), 3, Qt::DashDotLine));
    dashDotLinePaint.drawLine(5, 5, 88, 5);
    dashDotLinePaint.end();
    addItem(dashDotLinePixmap, "");
    
    // dash dot dot line
    QPixmap dashDotDotLinePixmap(QSize(100, 10));
    dashDotDotLinePixmap.fill(Qt::transparent);
    QPainter dashDotDotLinePaint(&dashDotDotLinePixmap);
    dashDotDotLinePaint.setPen(QPen(QBrush(Qt::black), 3, Qt::DashDotDotLine));
    dashDotDotLinePaint.drawLine(5, 5, 88, 5);
    dashDotDotLinePaint.end();
    addItem(dashDotDotLinePixmap, "");
    
    connect(this, QOverload<int>::of(&QComboBox::activated), this, [this] (const int& index) {
        std::vector<unsigned int>* dashArrayVector = new std::vector<unsigned int>();

        if (index == 1) {
            dashArrayVector->push_back(5);
            dashArrayVector->push_back(2);
        }
        else if (index == 2) {
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
        }
        else if (index == 3) {
            dashArrayVector->push_back(5);
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
        }
        else if (index == 4) {
            dashArrayVector->push_back(5);
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
            dashArrayVector->push_back(2);
        }
        
        emit dashArrayChosen(dashArrayVector);
    });
}

void MyDashArrayComboBox::setDashArray(std::vector<unsigned int>* dashArrayVector) {
    if (dashArrayVector) {
        if (dashArrayVector->size() == 2) {
            if (dashArrayVector->at(1) == 2) {
                if (dashArrayVector->at(0) == 5)
                    setCurrentIndex(1);
                else if (dashArrayVector->at(0) == 2)
                    setCurrentIndex(2);
            }
        }
        else if (dashArrayVector->size() == 4 && dashArrayVector->at(0) == 5 && dashArrayVector->at(1) == 2 && dashArrayVector->at(2) == 2 && dashArrayVector->at(3) == 2)
            setCurrentIndex(3);
        else if (dashArrayVector->size() == 6 && dashArrayVector->at(0) == 5 && dashArrayVector->at(1) == 2 && dashArrayVector->at(2) == 2 && dashArrayVector->at(3) == 2 && dashArrayVector->at(4) == 2 && dashArrayVector->at(5) == 2)
            setCurrentIndex(4);
    }
}

void MyDashArrayComboBox::resetValues() {
    setCurrentIndex(0);
}

// MyHeadPickerComboBox

MyHeadPickerComboBox::MyHeadPickerComboBox(QWidget* parent) : QComboBox(parent) {
    addItem("none");
    connect(this, QOverload<int>::of(&QComboBox::activated), this, [this] (const int& index) {
        if (index == count() - 1)
            emit headChosen("");
        else
            emit headChosen(itemText(index));
    });
}

void MyHeadPickerComboBox::enableItems(MainWindow* _mw) {
    int i = 0;
    for (MainWindow::constGLEndingIt gLEIt = _mw->gLEndingsBegin(); gLEIt != _mw->gLEndingsEnd(); ++gLEIt) {
        if ((*gLEIt)->isSetId()) {
            for (i = 0; i < count(); ++i) {
                if (itemText(i) == (*gLEIt)->getId().c_str())
                    break;
            }
            
            if (i == count())
                insertItem(count() - 1, QString((*gLEIt)->getId().c_str()));
        }
    }
}

void MyHeadPickerComboBox::setHead(const QString& head) {
    for (int i = 0; i < count(); ++i) {
        if (itemText(i) == head) {
            setCurrentIndex(i);
            break;
        }
    }
}

void MyHeadPickerComboBox::resetValues() {
    setCurrentIndex(count() - 1);
}

// MyDimensionRatioComboBox

MyDimensionRatioComboBox::MyDimensionRatioComboBox(QWidget* parent) : MyComboBox(parent) {
    setFixedHeight(18.0);
    setFixedWidth(70.0);
    addItem("N/A");
    for (double i = 0.2; i <= 5.0;) {
        addItem(QString::number(i));
        if (i < 1)
            i += 0.2;
        else
            i += 0.5;
    }
    
    connect(this, QOverload<int>::of(&QComboBox::activated), this, [this] (const int& index) {
        if (index == 0)
            emit ratioChosen(0.0000);
        else
            emit ratioChosen(itemText(index).toDouble());
    });
}

void MyDimensionRatioComboBox::setRatio(const double& ratio) {
    for (int i = 0; i < count(); ++i) {
        if (itemText(i) == QString::number(ratio)) {
            setCurrentIndex(i);
            break;
        }
    }
}

void MyDimensionRatioComboBox::resetValues() {
    setCurrentIndex(0);
}

// MyPauseOrResumeButton

MyPauseOrResumeButton::MyPauseOrResumeButton(QWidget* parent) : QPushButton(parent) {
    
}

void MyPauseOrResumeButton::enable() {
    setEnabled(true);
}

void MyPauseOrResumeButton::disable() {
    setEnabled(false);
    setText("Pause");
}

void MyPauseOrResumeButton::mousePressEvent(QMouseEvent *e) {
    QPushButton::mousePressEvent(e);
    if (text() == "Pause")
        pause();
    else if (text() == "Resume")
        resume();
    
}

void MyPauseOrResumeButton::pause() {
    setText("Resume");
    emit paused();
}

void MyPauseOrResumeButton::resume() {
    setText("Pause");
    emit resumed();
}

// MyColorTileButton

MyColorTileButton::MyColorTileButton(const QString& color, const QString& value, QWidget* parent) : QPushButton(parent) {
    _color = color;
    _value = value;
    setContentsMargins(0, 0, 0, 0);
    //setStyleSheet(" border-radius: 1px; background-color: " + _color + ";");
    setStyleSheet(" QPushButton {background-color: " + _color + "; border-radius: 1px;} QToolTip { background-color: white;}");
    setToolTip(_color);
    setFixedWidth(12);
    setFixedHeight(12);
}

const QString& MyColorTileButton::color() const {
    return _color;
}

const QString& MyColorTileButton::value() const {
    return _value;
}

// MyColorPickerMenu

MyColorPickerMenu::MyColorPickerMenu() {
    /// all colors
    QWidgetAction* allColors = new QWidgetAction(this);
    QWidget* allColorsWidget = new QWidget(this);
    QGridLayout* allColorsContentLayout = new QGridLayout(allColorsWidget);
    allColorsContentLayout->setSpacing(1);
    int rowCount = 2;
    int columnCount = 0;
    
    // label
    allColorsContentLayout->addWidget(new MyLabel("Colors"), allColorsContentLayout->rowCount() - 1, 0, 1, 14, Qt::AlignLeft);
    
    allColorsContentLayout->addItem(new QSpacerItem(1, 10), allColorsContentLayout->rowCount(), 0, 1, 14);

    // black
    MyColorTileButton* blackButton = new MyColorTileButton("Black", "#000000");
    allColorsContentLayout->addWidget(blackButton, rowCount++, columnCount);
    connect(blackButton, &QPushButton::clicked, this, [this, blackButton] () { this->colorTileButtonPicked(blackButton); });
    
    // darkslategray
    MyColorTileButton* darkslategrayButton = new MyColorTileButton("DarkSlateGray", "#2F4F4F");
    allColorsContentLayout->addWidget(darkslategrayButton, rowCount++, columnCount);
    connect(darkslategrayButton, &QPushButton::clicked, this, [this, darkslategrayButton] () { this->colorTileButtonPicked(darkslategrayButton); });
    
    // dimgray
    MyColorTileButton* dimgrayButton = new MyColorTileButton("DimGray" , "#696969");
    allColorsContentLayout->addWidget(dimgrayButton, rowCount++, columnCount);
    connect(dimgrayButton, &QPushButton::clicked, this, [this, dimgrayButton] () { this->colorTileButtonPicked(dimgrayButton); });
    
    // slategray
    MyColorTileButton* slategrayButton = new MyColorTileButton("SlateGray", "#708090");
    allColorsContentLayout->addWidget(slategrayButton, rowCount++, columnCount);
    connect(slategrayButton, &QPushButton::clicked, this, [this, slategrayButton] () { this->colorTileButtonPicked(slategrayButton); });
    
    // gray
    MyColorTileButton* grayButton = new MyColorTileButton("Gray", "#808080");
    allColorsContentLayout->addWidget(grayButton, rowCount++, columnCount);
    connect(grayButton, &QPushButton::clicked, this, [this, grayButton] () { this->colorTileButtonPicked(grayButton); });
    
    // lightslategray
    MyColorTileButton* lightslategrayButton = new MyColorTileButton("LightSlateGray", "#778899");
    allColorsContentLayout->addWidget(lightslategrayButton, rowCount++, columnCount);
    connect(lightslategrayButton, &QPushButton::clicked, this, [this, lightslategrayButton] () { this->colorTileButtonPicked(lightslategrayButton); });
    
    // darkgray
    MyColorTileButton* darkgrayButton = new MyColorTileButton("DarkGray", "#A9A9A9");
    allColorsContentLayout->addWidget(darkgrayButton, rowCount++, columnCount);
    connect(darkgrayButton, &QPushButton::clicked, this, [this, darkgrayButton] () { this->colorTileButtonPicked(darkgrayButton); });
    
    // silver
    MyColorTileButton* silverButton = new MyColorTileButton("Silver", "#C0C0C0");
    allColorsContentLayout->addWidget(silverButton, rowCount++, columnCount);
    connect(silverButton, &QPushButton::clicked, this, [this, silverButton] () { this->colorTileButtonPicked(silverButton); });
    
    // lightgray
    MyColorTileButton* lightgrayButton = new MyColorTileButton("LightGray", "#D3D3D3");
    allColorsContentLayout->addWidget(lightgrayButton, rowCount++, columnCount);
    connect(lightgrayButton, &QPushButton::clicked, this, [this, lightgrayButton] () { this->colorTileButtonPicked(lightgrayButton); });
    
    // gainsboro
    MyColorTileButton* gainsboroButton = new MyColorTileButton("Gainsboro", "#DCDCDC");
    allColorsContentLayout->addWidget(gainsboroButton, rowCount++, columnCount);
    connect(gainsboroButton, &QPushButton::clicked, this, [this, gainsboroButton] () { this->colorTileButtonPicked(gainsboroButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // maroon
    MyColorTileButton* maroonButton = new MyColorTileButton("Maroon", "#800000");
    allColorsContentLayout->addWidget(maroonButton, rowCount++, columnCount);
    connect(maroonButton, &QPushButton::clicked, this, [this, maroonButton] () { this->colorTileButtonPicked(maroonButton); });
    
    // brown
    MyColorTileButton* brownButton = new MyColorTileButton("Brown", "#A52A2A");
    allColorsContentLayout->addWidget(brownButton, rowCount++, columnCount);
    connect(brownButton, &QPushButton::clicked, this, [this, brownButton] () { this->colorTileButtonPicked(brownButton); });
    
    // saddlebrown
    MyColorTileButton* saddlebrownButton = new MyColorTileButton("SaddleBrown", "#8B4513");
    allColorsContentLayout->addWidget(saddlebrownButton, rowCount++, columnCount);
    connect(saddlebrownButton, &QPushButton::clicked, this, [this, saddlebrownButton] () { this->colorTileButtonPicked(saddlebrownButton); });
    
    // sienna
    MyColorTileButton* siennaButton = new MyColorTileButton("Sienna", "#A0522D");
    allColorsContentLayout->addWidget(siennaButton, rowCount++, columnCount);
    connect(siennaButton, &QPushButton::clicked, this, [this, siennaButton] () { this->colorTileButtonPicked(siennaButton); });
    
    // chocolate
    MyColorTileButton* chocolateButton = new MyColorTileButton("Chocolate", "#D2691E");
    allColorsContentLayout->addWidget(chocolateButton, rowCount++, columnCount);
    connect(chocolateButton, &QPushButton::clicked, this, [this, chocolateButton] () { this->colorTileButtonPicked(chocolateButton); });
    
    // darkgoldenrod
    MyColorTileButton* darkgoldenrodButton = new MyColorTileButton("DarkGoldenrod", "#B8860B");
    allColorsContentLayout->addWidget(darkgoldenrodButton, rowCount++, columnCount);
    connect(darkgoldenrodButton, &QPushButton::clicked, this, [this, darkgoldenrodButton] () { this->colorTileButtonPicked(darkgoldenrodButton); });
    
    // peru
    MyColorTileButton* peruButton = new MyColorTileButton("Peru", "#CD853F");
    allColorsContentLayout->addWidget(peruButton, rowCount++, columnCount);
    connect(peruButton, &QPushButton::clicked, this, [this, peruButton] () { this->colorTileButtonPicked(peruButton); });
    
    // rosybrown
    MyColorTileButton* rosybrownButton = new MyColorTileButton("RosyBrown", "#BC8F8F");
    allColorsContentLayout->addWidget(rosybrownButton, rowCount++, columnCount);
    connect(rosybrownButton, &QPushButton::clicked, this, [this, rosybrownButton] () { this->colorTileButtonPicked(rosybrownButton); });
    
    // goldenrod
    MyColorTileButton* goldenrodButton = new MyColorTileButton("Goldenrod", "#DAA520");
    allColorsContentLayout->addWidget(goldenrodButton, rowCount++, columnCount);
    connect(goldenrodButton, &QPushButton::clicked, this, [this, goldenrodButton] () { this->colorTileButtonPicked(goldenrodButton); });
    
    // sandybrown
    MyColorTileButton* sandybrownButton = new MyColorTileButton("SandyBrown", "#F4A460");
    allColorsContentLayout->addWidget(sandybrownButton, rowCount++, columnCount);
    connect(sandybrownButton, &QPushButton::clicked, this, [this, sandybrownButton] () { this->colorTileButtonPicked(sandybrownButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // tan
    MyColorTileButton* tanButton = new MyColorTileButton("Tan", "#D2B48C");
    allColorsContentLayout->addWidget(tanButton, rowCount++, columnCount);
    connect(tanButton, &QPushButton::clicked, this, [this, tanButton] () { this->colorTileButtonPicked(tanButton); });
    
    // burlywood
    MyColorTileButton* burlywoodButton = new MyColorTileButton("Burlywood", "#DEB887");
    allColorsContentLayout->addWidget(burlywoodButton, rowCount++, columnCount);
    connect(burlywoodButton, &QPushButton::clicked, this, [this, burlywoodButton] () { this->colorTileButtonPicked(burlywoodButton); });
    
    // wheat
    MyColorTileButton* wheatButton = new MyColorTileButton("Wheat", "#F5DEB3");
    allColorsContentLayout->addWidget(wheatButton, rowCount++, columnCount);
    connect(wheatButton, &QPushButton::clicked, this, [this, wheatButton] () { this->colorTileButtonPicked(wheatButton); });
    
    // navajowhite
    MyColorTileButton* navajowhiteButton = new MyColorTileButton("NavajoWhite", "#FFDEAD");
    allColorsContentLayout->addWidget(navajowhiteButton, rowCount++, columnCount);
    connect(navajowhiteButton, &QPushButton::clicked, this, [this, navajowhiteButton] () { this->colorTileButtonPicked(navajowhiteButton); });
    
    // bisque
    MyColorTileButton* bisqueButton = new MyColorTileButton("Bisque", "#FFE4C4");
    allColorsContentLayout->addWidget(bisqueButton, rowCount++, columnCount);
    connect(bisqueButton, &QPushButton::clicked, this, [this, bisqueButton] () { this->colorTileButtonPicked(bisqueButton); });
    
    // blanchedalmond
    MyColorTileButton* blanchedalmondButton = new MyColorTileButton("BlanchedAlmond", "#FFEBCD");
    allColorsContentLayout->addWidget(blanchedalmondButton, rowCount++, columnCount);
    connect(blanchedalmondButton, &QPushButton::clicked, this, [this, blanchedalmondButton] () { this->colorTileButtonPicked(blanchedalmondButton); });
    
    // cornsilk
    MyColorTileButton* cornsilkButton = new MyColorTileButton("Cornsilk", "#FFF8DC");
    allColorsContentLayout->addWidget(cornsilkButton, rowCount++, columnCount);
    connect(cornsilkButton, &QPushButton::clicked, this, [this, cornsilkButton] () { this->colorTileButtonPicked(cornsilkButton); });
    
    // darkred
    MyColorTileButton* darkredButton = new MyColorTileButton("DarkRed", "#8B0000");
    allColorsContentLayout->addWidget(darkredButton, rowCount++, columnCount);
    connect(darkredButton, &QPushButton::clicked, this, [this, darkredButton] () { this->colorTileButtonPicked(darkredButton); });
    
    // red
    MyColorTileButton* redButton = new MyColorTileButton("Red", "#FF0000");
    allColorsContentLayout->addWidget(redButton, rowCount++, columnCount);
    connect(redButton, &QPushButton::clicked, this, [this, redButton] () { this->colorTileButtonPicked(redButton); });
    
    // firebrick
    MyColorTileButton* firebrickButton = new MyColorTileButton("Firebrick", "#B22222");
    allColorsContentLayout->addWidget(firebrickButton, rowCount++, columnCount);
    connect(firebrickButton, &QPushButton::clicked, this, [this, firebrickButton] () { this->colorTileButtonPicked(firebrickButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // crimson
    MyColorTileButton* crimsonButton = new MyColorTileButton("Crimson", "#DC143C");
    allColorsContentLayout->addWidget(crimsonButton, rowCount++, columnCount);
    connect(crimsonButton, &QPushButton::clicked, this, [this, crimsonButton] () { this->colorTileButtonPicked(crimsonButton); });
    
    // indianred
    MyColorTileButton* indianredButton = new MyColorTileButton("IndianRed", "#CD5C5C");
    allColorsContentLayout->addWidget(indianredButton, rowCount++, columnCount);
    connect(indianredButton, &QPushButton::clicked, this, [this, indianredButton] () { this->colorTileButtonPicked(indianredButton); });
    
    // lightcoral
    MyColorTileButton* lightcoralButton = new MyColorTileButton("LightCoral", "#F08080");
    allColorsContentLayout->addWidget(lightcoralButton, rowCount++, columnCount);
    connect(lightcoralButton, &QPushButton::clicked, this, [this, lightcoralButton] () { this->colorTileButtonPicked(lightcoralButton); });
    
    // salmon
    MyColorTileButton* salmonButton = new MyColorTileButton("Salmon", "#FA8072");
    allColorsContentLayout->addWidget(salmonButton, rowCount++, columnCount);
    connect(salmonButton, &QPushButton::clicked, this, [this, salmonButton] () { this->colorTileButtonPicked(salmonButton); });
    
    // darksalmon
    MyColorTileButton* darksalmonButton = new MyColorTileButton("DarkSalmon", "#E9967A");
    allColorsContentLayout->addWidget(darksalmonButton, rowCount++, columnCount);
    connect(darksalmonButton, &QPushButton::clicked, this, [this, darksalmonButton] () { this->colorTileButtonPicked(darksalmonButton); });
    
    // lightsalmon
    MyColorTileButton* lightsalmonButton = new MyColorTileButton("LightSalmon", "#FFA07A");
    allColorsContentLayout->addWidget(lightsalmonButton, rowCount++, columnCount);
    connect(lightsalmonButton, &QPushButton::clicked, this, [this, lightsalmonButton] () { this->colorTileButtonPicked(lightsalmonButton); });
    
    // orangered
    MyColorTileButton* orangeredButton = new MyColorTileButton("OrangeRed", "#FF4500");
    allColorsContentLayout->addWidget(orangeredButton, rowCount++, columnCount);
    connect(orangeredButton, &QPushButton::clicked, this, [this, orangeredButton] () { this->colorTileButtonPicked(orangeredButton); });
    
    // tomato
    MyColorTileButton* tomatoButton = new MyColorTileButton("Tomato", "#FF6347");
    allColorsContentLayout->addWidget(tomatoButton, rowCount++, columnCount);
    connect(tomatoButton, &QPushButton::clicked, this, [this, tomatoButton] () { this->colorTileButtonPicked(tomatoButton); });
    
    // darkorange
    MyColorTileButton* darkorangeButton = new MyColorTileButton("DarkOrange", "#FF8C00");
    allColorsContentLayout->addWidget(darkorangeButton, rowCount++, columnCount);
    connect(darkorangeButton, &QPushButton::clicked, this, [this, darkorangeButton] () { this->colorTileButtonPicked(darkorangeButton); });
    
    // coral
    MyColorTileButton* coralButton = new MyColorTileButton("Coral", "#FF7F50");
    allColorsContentLayout->addWidget(coralButton, rowCount++, columnCount);
    connect(coralButton, &QPushButton::clicked, this, [this, coralButton] () { this->colorTileButtonPicked(coralButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // orange
    MyColorTileButton* orangeButton = new MyColorTileButton("Orange", "#FFA500");
    allColorsContentLayout->addWidget(orangeButton, rowCount++, columnCount);
    connect(orangeButton, &QPushButton::clicked, this, [this, orangeButton] () { this->colorTileButtonPicked(orangeButton); });
    
    // mediumvioletred
    MyColorTileButton* mediumvioletredButton = new MyColorTileButton("MediumVioletRed", "#C71585");
    allColorsContentLayout->addWidget(mediumvioletredButton, rowCount++, columnCount);
    connect(mediumvioletredButton, &QPushButton::clicked, this, [this, mediumvioletredButton] () { this->colorTileButtonPicked(mediumvioletredButton); });
    
    // deeppink
    MyColorTileButton* deeppinkButton = new MyColorTileButton("DeepPink", "#FF1493");
    allColorsContentLayout->addWidget(deeppinkButton, rowCount++, columnCount);
    connect(deeppinkButton, &QPushButton::clicked, this, [this, deeppinkButton] () { this->colorTileButtonPicked(deeppinkButton); });
    
    // palevioletred
    MyColorTileButton* palevioletredButton = new MyColorTileButton("PaleVioletRed", "#DB7093");
    allColorsContentLayout->addWidget(palevioletredButton, rowCount++, columnCount);
    connect(palevioletredButton, &QPushButton::clicked, this, [this, palevioletredButton] () { this->colorTileButtonPicked(palevioletredButton); });
    
    // hotpink
    MyColorTileButton* hotpinkButton = new MyColorTileButton("HotPink", "#DB7093");
    allColorsContentLayout->addWidget(hotpinkButton, rowCount++, columnCount);
    connect(hotpinkButton, &QPushButton::clicked, this, [this, hotpinkButton] () { this->colorTileButtonPicked(hotpinkButton); });
    
    // lightpink
    MyColorTileButton* lightpinkButton = new MyColorTileButton("LightPink", "#FFB6C1");
    allColorsContentLayout->addWidget(lightpinkButton, rowCount++, columnCount);
    connect(lightpinkButton, &QPushButton::clicked, this, [this, lightpinkButton] () { this->colorTileButtonPicked(lightpinkButton); });
    
    // pink
    MyColorTileButton* pinkButton = new MyColorTileButton("Pink", "#FFC0CB");
    allColorsContentLayout->addWidget(pinkButton, rowCount++, columnCount);
    connect(pinkButton, &QPushButton::clicked, this, [this, pinkButton] () { this->colorTileButtonPicked(pinkButton); });
    
    // indigo
    MyColorTileButton* indigoButton = new MyColorTileButton("Indigo", "#4B0082");
    allColorsContentLayout->addWidget(indigoButton, rowCount++, columnCount);
    connect(indigoButton, &QPushButton::clicked, this, [this, indigoButton] () { this->colorTileButtonPicked(indigoButton); });
    
    // purple
    MyColorTileButton* purpleButton = new MyColorTileButton("Purple", "#800080");
    allColorsContentLayout->addWidget(purpleButton, rowCount++, columnCount);
    connect(purpleButton, &QPushButton::clicked, this, [this, purpleButton] () { this->colorTileButtonPicked(purpleButton); });
    
    // rebeccapurple
    //MyColorTileButton* rebeccapurpleButton = new MyColorTileButton("RebeccaPurple", "#663399");
    //allColorsContentLayout->addWidget(rebeccapurpleButton, rowCount++, columnCount);
    //connect(rebeccapurpleButton, &QPushButton::clicked, this, [this, rebeccapurpleButton] () { this->colorTileButtonPicked(rebeccapurpleButton); });
    
    // darkmagenta
    MyColorTileButton* darkmagentaButton = new MyColorTileButton("DarkMagenta", "#800080");
    allColorsContentLayout->addWidget(darkmagentaButton, rowCount++, columnCount);
    connect(darkmagentaButton, &QPushButton::clicked, this, [this, darkmagentaButton] () { this->colorTileButtonPicked(darkmagentaButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // darkviolet
    MyColorTileButton* darkvioletButton = new MyColorTileButton("DarkViolet", "#9400D3");
    allColorsContentLayout->addWidget(darkvioletButton, rowCount++, columnCount);
    connect(darkvioletButton, &QPushButton::clicked, this, [this, darkvioletButton] () { this->colorTileButtonPicked(darkvioletButton); });
    
    // darkslateblue
    MyColorTileButton* darkslateblueButton = new MyColorTileButton("DarkSlateBlue", "#483D8B");
    allColorsContentLayout->addWidget(darkslateblueButton, rowCount++, columnCount);
    connect(darkslateblueButton, &QPushButton::clicked, this, [this, darkslateblueButton] () { this->colorTileButtonPicked(darkslateblueButton); });
    
    // blueviolet
    MyColorTileButton* bluevioletButton = new MyColorTileButton("BlueViolet", "#8A2BE2");
    allColorsContentLayout->addWidget(bluevioletButton, rowCount++, columnCount);
    connect(bluevioletButton, &QPushButton::clicked, this, [this, bluevioletButton] () { this->colorTileButtonPicked(bluevioletButton); });
    
    // darkorchid
    MyColorTileButton* darkorchidButton = new MyColorTileButton("DarkOrchid", "#9932CC");
    allColorsContentLayout->addWidget(darkorchidButton, rowCount++, columnCount);
    connect(darkorchidButton, &QPushButton::clicked, this, [this, darkorchidButton] () { this->colorTileButtonPicked(darkorchidButton); });
    
    // fuchsia
    MyColorTileButton* fuchsiaButton = new MyColorTileButton("Fuchsia", "#FF00FF");
    allColorsContentLayout->addWidget(fuchsiaButton, rowCount++, columnCount);
    connect(fuchsiaButton, &QPushButton::clicked, this, [this, fuchsiaButton] () { this->colorTileButtonPicked(fuchsiaButton); });
    
    // magenta
    MyColorTileButton* magentaButton = new MyColorTileButton("Magenta", "#FF00FF");
    allColorsContentLayout->addWidget(magentaButton, rowCount++, columnCount);
    connect(magentaButton, &QPushButton::clicked, this, [this, magentaButton] () { this->colorTileButtonPicked(magentaButton); });
    
    // slateblue
    MyColorTileButton* slateblueButton = new MyColorTileButton("SlateBlue", "#6A5ACD");
    allColorsContentLayout->addWidget(slateblueButton, rowCount++, columnCount);
    connect(slateblueButton, &QPushButton::clicked, this, [this, slateblueButton] () { this->colorTileButtonPicked(slateblueButton); });
    
    // mediumslateblue
    MyColorTileButton* mediumslateblueButton = new MyColorTileButton("MediumSlateBlue", "#7B68EE");
    allColorsContentLayout->addWidget(mediumslateblueButton, rowCount++, columnCount);
    connect(mediumslateblueButton, &QPushButton::clicked, this, [this, mediumslateblueButton] () { this->colorTileButtonPicked(mediumslateblueButton); });
    
    // mediumorchid
    MyColorTileButton* mediumorchidButton = new MyColorTileButton("MediumOrchid", "#BA55D3");
    allColorsContentLayout->addWidget(mediumorchidButton, rowCount++, columnCount);
    connect(mediumorchidButton, &QPushButton::clicked, this, [this, mediumorchidButton] () { this->colorTileButtonPicked(mediumorchidButton); });
    
    // mediumpurple
    MyColorTileButton* mediumpurpleButton = new MyColorTileButton("MediumPurple", "#9370DB");
    allColorsContentLayout->addWidget(mediumpurpleButton, rowCount++, columnCount);
    connect(mediumpurpleButton, &QPushButton::clicked, this, [this, mediumpurpleButton] () { this->colorTileButtonPicked(mediumpurpleButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // orchid
    MyColorTileButton* orchidButton = new MyColorTileButton("Orchid", "#DA70D6");
    allColorsContentLayout->addWidget(orchidButton, rowCount++, columnCount);
    connect(orchidButton, &QPushButton::clicked, this, [this, orchidButton] () { this->colorTileButtonPicked(orchidButton); });
    
    // violet
    MyColorTileButton* violetButton = new MyColorTileButton("Violet", "#EE82EE");
    allColorsContentLayout->addWidget(violetButton, rowCount++, columnCount);
    connect(violetButton, &QPushButton::clicked, this, [this, violetButton] () { this->colorTileButtonPicked(violetButton); });
    
    // plum
    MyColorTileButton* plumButton = new MyColorTileButton("Plum", "#DDA0DD");
    allColorsContentLayout->addWidget(plumButton, rowCount++, columnCount);
    connect(plumButton, &QPushButton::clicked, this, [this, plumButton] () { this->colorTileButtonPicked(plumButton); });
    
    // thistle
    MyColorTileButton* thistleButton = new MyColorTileButton("Thistle", "#D8BFD8");
    allColorsContentLayout->addWidget(thistleButton, rowCount++, columnCount);
    connect(thistleButton, &QPushButton::clicked, this, [this, thistleButton] () { this->colorTileButtonPicked(thistleButton); });
    
    // lavender
    MyColorTileButton* lavenderButton = new MyColorTileButton("Lavender", "#E6E6FA");
    allColorsContentLayout->addWidget(lavenderButton, rowCount++, columnCount);
    connect(lavenderButton, &QPushButton::clicked, this, [this, lavenderButton] () { this->colorTileButtonPicked(lavenderButton); });
    
    // navy
    MyColorTileButton* navyButton = new MyColorTileButton("Navy", "#000080");
    allColorsContentLayout->addWidget(navyButton, rowCount++, columnCount);
    connect(navyButton, &QPushButton::clicked, this, [this, navyButton] () { this->colorTileButtonPicked(navyButton); });
    
    // darkblue
    MyColorTileButton* darkblueButton = new MyColorTileButton("DarkBlue", "#00008B");
    allColorsContentLayout->addWidget(darkblueButton, rowCount++, columnCount);
    connect(darkblueButton, &QPushButton::clicked, this, [this, darkblueButton] () { this->colorTileButtonPicked(darkblueButton); });
    
    // mediumblue
    MyColorTileButton* mediumblueButton = new MyColorTileButton("MediumBlue", "#0000CD");
    allColorsContentLayout->addWidget(mediumblueButton, rowCount++, columnCount);
    connect(mediumblueButton, &QPushButton::clicked, this, [this, mediumblueButton] () { this->colorTileButtonPicked(mediumblueButton); });
    
    // blue
    MyColorTileButton* blueButton = new MyColorTileButton("Blue", "#0000FF");
    allColorsContentLayout->addWidget(blueButton, rowCount++, columnCount);
    connect(blueButton, &QPushButton::clicked, this, [this, blueButton] () { this->colorTileButtonPicked(blueButton); });
    
    // midnightblue
    MyColorTileButton* midnightblueButton = new MyColorTileButton("MidnightBlue", "#191970");
    allColorsContentLayout->addWidget(midnightblueButton, rowCount++, columnCount);
    connect(midnightblueButton, &QPushButton::clicked, this, [this, midnightblueButton] () { this->colorTileButtonPicked(midnightblueButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // royalblue
    MyColorTileButton* royalblueButton = new MyColorTileButton("RoyalBlue", "#4169E1");
    allColorsContentLayout->addWidget(royalblueButton, rowCount++, columnCount);
    connect(royalblueButton, &QPushButton::clicked, this, [this, royalblueButton] () { this->colorTileButtonPicked(royalblueButton); });
    
    // steelblue
    MyColorTileButton* steelblueButton = new MyColorTileButton("SteelBlue", "#4682B4");
    allColorsContentLayout->addWidget(steelblueButton, rowCount++, columnCount);
    connect(steelblueButton, &QPushButton::clicked, this, [this, steelblueButton] () { this->colorTileButtonPicked(steelblueButton); });
    
    // dodgerblue
    MyColorTileButton* dodgerblueButton = new MyColorTileButton("DodgerBlue", "#1E90FF");
    allColorsContentLayout->addWidget(dodgerblueButton, rowCount++, columnCount);
    connect(dodgerblueButton, &QPushButton::clicked, this, [this, dodgerblueButton] () { this->colorTileButtonPicked(dodgerblueButton); });
    
    // deepskyblue
    MyColorTileButton* deepskyblueButton = new MyColorTileButton("DeepSkyBlue", "#00BFFF");
    allColorsContentLayout->addWidget(deepskyblueButton, rowCount++, columnCount);
    connect(deepskyblueButton, &QPushButton::clicked, this, [this, deepskyblueButton] () { this->colorTileButtonPicked(deepskyblueButton); });
    
    // cornflowerblue
    MyColorTileButton* cornflowerblueButton = new MyColorTileButton("CornflowerBlue", "#6495ED");
    allColorsContentLayout->addWidget(cornflowerblueButton, rowCount++, columnCount);
    connect(cornflowerblueButton, &QPushButton::clicked, this, [this, cornflowerblueButton] () { this->colorTileButtonPicked(cornflowerblueButton); });
    
    // skyblue
    MyColorTileButton* skyblueButton = new MyColorTileButton("SkyBlue", "#87CEEB");
    allColorsContentLayout->addWidget(skyblueButton, rowCount++, columnCount);
    connect(skyblueButton, &QPushButton::clicked, this, [this, skyblueButton] () { this->colorTileButtonPicked(skyblueButton); });
    
    // lightskyblue
    MyColorTileButton* lightskyblueButton = new MyColorTileButton("LightSkyBlue", "#87CEFA");
    allColorsContentLayout->addWidget(lightskyblueButton, rowCount++, columnCount);
    connect(lightskyblueButton, &QPushButton::clicked, this, [this, lightskyblueButton] () { this->colorTileButtonPicked(lightskyblueButton); });
    
    // lightsteelblue
    MyColorTileButton* lightsteelblueButton = new MyColorTileButton("LightSteelBlue", "#B0C4DE");
    allColorsContentLayout->addWidget(lightsteelblueButton, rowCount++, columnCount);
    connect(lightsteelblueButton, &QPushButton::clicked, this, [this, lightsteelblueButton] () { this->colorTileButtonPicked(lightsteelblueButton); });
    
    // lightblue
    MyColorTileButton* lightblueButton = new MyColorTileButton("LightBlue", "#ADD8E6");
    allColorsContentLayout->addWidget(lightblueButton, rowCount++, columnCount);
    connect(lightblueButton, &QPushButton::clicked, this, [this, lightblueButton] () { this->colorTileButtonPicked(lightblueButton); });
    
    // powderblue
    MyColorTileButton* powderblueButton = new MyColorTileButton("PowderBlue", "#B0E0E6");
    allColorsContentLayout->addWidget(powderblueButton, rowCount++, columnCount);
    connect(powderblueButton, &QPushButton::clicked, this, [this, powderblueButton] () { this->colorTileButtonPicked(powderblueButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // teal
    MyColorTileButton* tealButton = new MyColorTileButton("Teal", "#008080");
    allColorsContentLayout->addWidget(tealButton, rowCount++, columnCount);
    connect(tealButton, &QPushButton::clicked, this, [this, tealButton] () { this->colorTileButtonPicked(tealButton); });
    
    // darkcyan
    MyColorTileButton* darkcyanButton = new MyColorTileButton("DarkCyan", "#008B8B");
    allColorsContentLayout->addWidget(darkcyanButton, rowCount++, columnCount);
    connect(darkcyanButton, &QPushButton::clicked, this, [this, darkcyanButton] () { this->colorTileButtonPicked(darkcyanButton); });
    
    // lightseagreen
    MyColorTileButton* lightseagreenButton = new MyColorTileButton("LightSeaGreen", "#20B2AA");
    allColorsContentLayout->addWidget(lightseagreenButton, rowCount++, columnCount);
    connect(lightseagreenButton, &QPushButton::clicked, this, [this, lightseagreenButton] () { this->colorTileButtonPicked(lightseagreenButton); });
    
    // cadetblue
    MyColorTileButton* cadetblueButton = new MyColorTileButton("CadetBlue", "#5F9EA0");
    allColorsContentLayout->addWidget(cadetblueButton, rowCount++, columnCount);
    connect(cadetblueButton, &QPushButton::clicked, this, [this, cadetblueButton] () { this->colorTileButtonPicked(cadetblueButton); });
    
    // darkturquoise
    MyColorTileButton* darkturquoiseButton = new MyColorTileButton("DarkTurquoise", "#00CED1");
    allColorsContentLayout->addWidget(darkturquoiseButton, rowCount++, columnCount);
    connect(darkturquoiseButton, &QPushButton::clicked, this, [this, darkturquoiseButton] () { this->colorTileButtonPicked(darkturquoiseButton); });
    
    // mediumturquoise
    MyColorTileButton* mediumturquoiseButton = new MyColorTileButton("MediumTurquoise", "#48D1CC");
    allColorsContentLayout->addWidget(mediumturquoiseButton, rowCount++, columnCount);
    connect(mediumturquoiseButton, &QPushButton::clicked, this, [this, mediumturquoiseButton] () { this->colorTileButtonPicked(mediumturquoiseButton); });
    
    // turquoise
    MyColorTileButton* turquoiseButton = new MyColorTileButton("Turquoise", "#40E0D0");
    allColorsContentLayout->addWidget(turquoiseButton, rowCount++, columnCount);
    connect(turquoiseButton, &QPushButton::clicked, this, [this, turquoiseButton] () { this->colorTileButtonPicked(turquoiseButton); });
    
    // aqua
    MyColorTileButton* aquaButton = new MyColorTileButton("Aqua", "#00FFFF");
    allColorsContentLayout->addWidget(aquaButton, rowCount++, columnCount);
    connect(aquaButton, &QPushButton::clicked, this, [this, aquaButton] () { this->colorTileButtonPicked(aquaButton); });
    
    // cyan
    MyColorTileButton* cyanButton = new MyColorTileButton("Cyan", "#00FFFF");
    allColorsContentLayout->addWidget(cyanButton, rowCount++, columnCount);
    connect(cyanButton, &QPushButton::clicked, this, [this, cyanButton] () { this->colorTileButtonPicked(cyanButton); });
    
    // aquamarine
    MyColorTileButton* aquamarineButton = new MyColorTileButton("Aquamarine", "#7FFFD4");
    allColorsContentLayout->addWidget(aquamarineButton, rowCount++, columnCount);
    connect(aquamarineButton, &QPushButton::clicked, this, [this, aquamarineButton] () { this->colorTileButtonPicked(aquamarineButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // paleturquoise
    MyColorTileButton* paleturquoiseButton = new MyColorTileButton("PaleTurquoise", "#AFEEEE");
    allColorsContentLayout->addWidget(paleturquoiseButton, rowCount++, columnCount);
    connect(paleturquoiseButton, &QPushButton::clicked, this, [this, paleturquoiseButton] () { this->colorTileButtonPicked(paleturquoiseButton); });
    
    // lightcyan
    MyColorTileButton* lightcyanButton = new MyColorTileButton("LightCyan", "#E0FFFF");
    allColorsContentLayout->addWidget(lightcyanButton, rowCount++, columnCount);
    connect(lightcyanButton, &QPushButton::clicked, this, [this, lightcyanButton] () { this->colorTileButtonPicked(lightcyanButton); });
    
    // darkgreen
    MyColorTileButton* darkgreenButton = new MyColorTileButton("DarkGreen", "#006400");
    allColorsContentLayout->addWidget(darkgreenButton, rowCount++, columnCount);
    connect(darkgreenButton, &QPushButton::clicked, this, [this, darkgreenButton] () { this->colorTileButtonPicked(darkgreenButton); });
    
    // green
    MyColorTileButton* greenButton = new MyColorTileButton("Green", "#008000");
    allColorsContentLayout->addWidget(greenButton, rowCount++, columnCount);
    connect(greenButton, &QPushButton::clicked, this, [this, greenButton] () { this->colorTileButtonPicked(greenButton); });
    
    // darkolivegreen
    MyColorTileButton* darkolivegreenButton = new MyColorTileButton("DarkOliveGreen", "#556B2F");
    allColorsContentLayout->addWidget(darkolivegreenButton, rowCount++, columnCount);
    connect(darkolivegreenButton, &QPushButton::clicked, this, [this, darkolivegreenButton] () { this->colorTileButtonPicked(darkolivegreenButton); });
    
    // forestgreen
    MyColorTileButton* forestgreenButton = new MyColorTileButton("ForestGreen", "#228B22");
    allColorsContentLayout->addWidget(forestgreenButton, rowCount++, columnCount);
    connect(forestgreenButton, &QPushButton::clicked, this, [this, forestgreenButton] () { this->colorTileButtonPicked(forestgreenButton); });
    
    // seagreen
    MyColorTileButton* seagreenButton = new MyColorTileButton("SeaGreen", "#2E8B57");
    allColorsContentLayout->addWidget(seagreenButton, rowCount++, columnCount);
    connect(seagreenButton, &QPushButton::clicked, this, [this, seagreenButton] () { this->colorTileButtonPicked(seagreenButton); });
    
    // olive
    MyColorTileButton* oliveButton = new MyColorTileButton("Olive", "#808000");
    allColorsContentLayout->addWidget(oliveButton, rowCount++, columnCount);
    connect(oliveButton, &QPushButton::clicked, this, [this, oliveButton] () { this->colorTileButtonPicked(oliveButton); });
    
    // olivedrab
    MyColorTileButton* olivedrabButton = new MyColorTileButton("OliveDrab", "#6B8E23");
    allColorsContentLayout->addWidget(olivedrabButton, rowCount++, columnCount);
    connect(olivedrabButton, &QPushButton::clicked, this, [this, olivedrabButton] () { this->colorTileButtonPicked(olivedrabButton); });
    
    // mediumseagreen
    MyColorTileButton* mediumseagreenButton = new MyColorTileButton("MediumSeaGreen", "#3CB371");
    allColorsContentLayout->addWidget(mediumseagreenButton, rowCount++, columnCount);
    connect(mediumseagreenButton, &QPushButton::clicked, this, [this, mediumseagreenButton] () { this->colorTileButtonPicked(mediumseagreenButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // limegreen
    MyColorTileButton* limegreenButton = new MyColorTileButton("LimeGreen", "#32CD32");
    allColorsContentLayout->addWidget(limegreenButton, rowCount++, columnCount);
    connect(limegreenButton, &QPushButton::clicked, this, [this, limegreenButton] () { this->colorTileButtonPicked(limegreenButton); });
    
    // lime
    MyColorTileButton* limeButton = new MyColorTileButton("Lime", "#00FF00");
    allColorsContentLayout->addWidget(limeButton, rowCount++, columnCount);
    connect(limeButton, &QPushButton::clicked, this, [this, limeButton] () { this->colorTileButtonPicked(limeButton); });
    
    // springgreen
    MyColorTileButton* springgreenButton = new MyColorTileButton("SpringGreen", "#00FF7F");
    allColorsContentLayout->addWidget(springgreenButton, rowCount++, columnCount);
    connect(springgreenButton, &QPushButton::clicked, this, [this, springgreenButton] () { this->colorTileButtonPicked(springgreenButton); });
    
    // mediumspringgreen
    MyColorTileButton* mediumspringgreenButton = new MyColorTileButton("MediumSpringGreen", "#00FA9A");
    allColorsContentLayout->addWidget(mediumspringgreenButton, rowCount++, columnCount);
    connect(mediumspringgreenButton, &QPushButton::clicked, this, [this, mediumspringgreenButton] () { this->colorTileButtonPicked(mediumspringgreenButton); });
    
    // darkseagreen
    MyColorTileButton* darkseagreenButton = new MyColorTileButton("DarkSeaGreen", "#8FBC8F");
    allColorsContentLayout->addWidget(darkseagreenButton, rowCount++, columnCount);
    connect(darkseagreenButton, &QPushButton::clicked, this, [this, darkseagreenButton] () { this->colorTileButtonPicked(darkseagreenButton); });
    
    // mediumaquamarine
    MyColorTileButton* mediumaquamarineButton = new MyColorTileButton("MediumAquamarine", "#66CDAA");
    allColorsContentLayout->addWidget(mediumaquamarineButton, rowCount++, columnCount);
    connect(mediumaquamarineButton, &QPushButton::clicked, this, [this, mediumaquamarineButton] () { this->colorTileButtonPicked(mediumaquamarineButton); });
    
    // yellowgreen
    MyColorTileButton* yellowgreenButton = new MyColorTileButton("YellowGreen", "#9ACD32");
    allColorsContentLayout->addWidget(yellowgreenButton, rowCount++, columnCount);
    connect(yellowgreenButton, &QPushButton::clicked, this, [this, yellowgreenButton] () { this->colorTileButtonPicked(yellowgreenButton); });
    
    // lawngreen
    MyColorTileButton* lawngreenButton = new MyColorTileButton("LawnGreen", "#7CFC00");
    allColorsContentLayout->addWidget(lawngreenButton, rowCount++, columnCount);
    connect(lawngreenButton, &QPushButton::clicked, this, [this, lawngreenButton] () { this->colorTileButtonPicked(lawngreenButton); });
    
    // chartreuse
    MyColorTileButton* chartreuseButton = new MyColorTileButton("Chartreuse", "#7FFF00");
    allColorsContentLayout->addWidget(chartreuseButton, rowCount++, columnCount);
    connect(chartreuseButton, &QPushButton::clicked, this, [this, chartreuseButton] () { this->colorTileButtonPicked(chartreuseButton); });
    
    // lightgreen
    MyColorTileButton* lightgreenButton = new MyColorTileButton("LightGreen", "#90EE90");
    allColorsContentLayout->addWidget(lightgreenButton, rowCount++, columnCount);
    connect(lightgreenButton, &QPushButton::clicked, this, [this, lightgreenButton] () { this->colorTileButtonPicked(lightgreenButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // greenyellow
    MyColorTileButton* greenyellowButton = new MyColorTileButton("GreenYellow", "#ADFF2F");
    allColorsContentLayout->addWidget(greenyellowButton, rowCount++, columnCount);
    connect(greenyellowButton, &QPushButton::clicked, this, [this, greenyellowButton] () { this->colorTileButtonPicked(greenyellowButton); });
    
    // palegreen
    MyColorTileButton* palegreenButton = new MyColorTileButton("PaleGreen", "#98FB98");
    allColorsContentLayout->addWidget(palegreenButton, rowCount++, columnCount);
    connect(palegreenButton, &QPushButton::clicked, this, [this, palegreenButton] () { this->colorTileButtonPicked(palegreenButton); });
    
    // darkkhaki
    MyColorTileButton* darkkhakiButton = new MyColorTileButton("DarkKhaki", "#BDB76B");
    allColorsContentLayout->addWidget(darkkhakiButton, rowCount++, columnCount);
    connect(darkkhakiButton, &QPushButton::clicked, this, [this, darkkhakiButton] () { this->colorTileButtonPicked(darkkhakiButton); });
    
    // gold
    MyColorTileButton* goldButton = new MyColorTileButton("Gold", "#FFD700");
    allColorsContentLayout->addWidget(goldButton, rowCount++, columnCount);
    connect(goldButton, &QPushButton::clicked, this, [this, goldButton] () { this->colorTileButtonPicked(goldButton); });
    
    // khaki
    MyColorTileButton* khakiButton = new MyColorTileButton("Khaki", "#F0E68C");
    allColorsContentLayout->addWidget(khakiButton, rowCount++, columnCount);
    connect(khakiButton, &QPushButton::clicked, this, [this, khakiButton] () { this->colorTileButtonPicked(khakiButton); });
    
    // peachpuff
    MyColorTileButton* peachpuffButton = new MyColorTileButton("PeachPuff", "#FFDAB9");
    allColorsContentLayout->addWidget(peachpuffButton, rowCount++, columnCount);
    connect(peachpuffButton, &QPushButton::clicked, this, [this, peachpuffButton] () { this->colorTileButtonPicked(peachpuffButton); });
    
    // yellow
    MyColorTileButton* yellowButton = new MyColorTileButton("Yellow", "#FFFF00");
    allColorsContentLayout->addWidget(yellowButton, rowCount++, columnCount);
    connect(yellowButton, &QPushButton::clicked, this, [this, yellowButton] () { this->colorTileButtonPicked(yellowButton); });
    
    // palegoldenrod
    MyColorTileButton* palegoldenrodButton = new MyColorTileButton("PaleGoldenrod", "#EEE8AA");
    allColorsContentLayout->addWidget(palegoldenrodButton, rowCount++, columnCount);
    connect(palegoldenrodButton, &QPushButton::clicked, this, [this, palegoldenrodButton] () { this->colorTileButtonPicked(palegoldenrodButton); });
    
    // moccasin
    MyColorTileButton* moccasinButton = new MyColorTileButton("Moccasin", "#FFE4B5");
    allColorsContentLayout->addWidget(moccasinButton, rowCount++, columnCount);
    connect(moccasinButton, &QPushButton::clicked, this, [this, moccasinButton] () { this->colorTileButtonPicked(moccasinButton); });
    
    // papayawhip
    MyColorTileButton* papayawhipButton = new MyColorTileButton("PapayaWhip", "#FFEFD5");
    allColorsContentLayout->addWidget(papayawhipButton, rowCount++, columnCount);
    connect(papayawhipButton, &QPushButton::clicked, this, [this, papayawhipButton] () { this->colorTileButtonPicked(papayawhipButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // lightgoldenrodyellow
    MyColorTileButton* lightgoldenrodyellowButton = new MyColorTileButton("LightGoldenrodYellow", "#FAFAD2");
    allColorsContentLayout->addWidget(lightgoldenrodyellowButton, rowCount++, columnCount);
    connect(lightgoldenrodyellowButton, &QPushButton::clicked, this, [this, lightgoldenrodyellowButton] () { this->colorTileButtonPicked(lightgoldenrodyellowButton); });
    
    // lemonchiffon
    MyColorTileButton* lemonchiffonButton = new MyColorTileButton("LemonChiffon", "#FFFACD");
    allColorsContentLayout->addWidget(lemonchiffonButton, rowCount++, columnCount);
    connect(lemonchiffonButton, &QPushButton::clicked, this, [this, lemonchiffonButton] () { this->colorTileButtonPicked(lemonchiffonButton); });
    
    // lightyellow
    MyColorTileButton* lightyellowButton = new MyColorTileButton("LightYellow", "#FFFFE0");
    allColorsContentLayout->addWidget(lightyellowButton, rowCount++, columnCount);
    connect(lightyellowButton, &QPushButton::clicked, this, [this, lightyellowButton] () { this->colorTileButtonPicked(lightyellowButton); });
    
    // mistyrose
    MyColorTileButton* mistyroseButton = new MyColorTileButton("MistyRose", "#FFE4E1");
    allColorsContentLayout->addWidget(mistyroseButton, rowCount++, columnCount);
    connect(mistyroseButton, &QPushButton::clicked, this, [this, mistyroseButton] () { this->colorTileButtonPicked(mistyroseButton); });
    
    // antiquewhite
    MyColorTileButton* antiquewhiteButton = new MyColorTileButton("AntiqueWhite", "#FAEBD7");
    allColorsContentLayout->addWidget(antiquewhiteButton, rowCount++, columnCount);
    connect(antiquewhiteButton, &QPushButton::clicked, this, [this, antiquewhiteButton] () { this->colorTileButtonPicked(antiquewhiteButton); });
    
    // linen
    MyColorTileButton* linenButton = new MyColorTileButton("Linen", "#FAF0E6");
    allColorsContentLayout->addWidget(linenButton, rowCount++, columnCount);
    connect(linenButton, &QPushButton::clicked, this, [this, linenButton] () { this->colorTileButtonPicked(linenButton); });
    
    // beige
    MyColorTileButton* beigeButton = new MyColorTileButton("Beige", "#F5F5DC");
    allColorsContentLayout->addWidget(beigeButton, rowCount++, columnCount);
    connect(beigeButton, &QPushButton::clicked, this, [this, beigeButton] () { this->colorTileButtonPicked(beigeButton); });
    
    // whitesmoke
    MyColorTileButton* whitesmokeButton = new MyColorTileButton("WhiteSmoke", "#F5F5F5");
    allColorsContentLayout->addWidget(whitesmokeButton, rowCount++, columnCount);
    connect(whitesmokeButton, &QPushButton::clicked, this, [this, whitesmokeButton] () { this->colorTileButtonPicked(whitesmokeButton); });
    
    // lavenderblush
    MyColorTileButton* lavenderblushButton = new MyColorTileButton("LavenderBlush", "#FFF0F5");
    allColorsContentLayout->addWidget(lavenderblushButton, rowCount++, columnCount);
    connect(lavenderblushButton, &QPushButton::clicked, this, [this, lavenderblushButton] () { this->colorTileButtonPicked(lavenderblushButton); });
    
    // oldlace
    MyColorTileButton* oldlaceButton = new MyColorTileButton("OldLace", "#FDF5E6");
    allColorsContentLayout->addWidget(oldlaceButton, rowCount++, columnCount);
    connect(oldlaceButton, &QPushButton::clicked, this, [this, oldlaceButton] () { this->colorTileButtonPicked(oldlaceButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // aliceblue
    MyColorTileButton* aliceblueButton = new MyColorTileButton("AliceBlue", "#F0F8FF");
    allColorsContentLayout->addWidget(aliceblueButton, rowCount++, columnCount);
    connect(aliceblueButton, &QPushButton::clicked, this, [this, aliceblueButton] () { this->colorTileButtonPicked(aliceblueButton); });
    
    // seashell
    MyColorTileButton* seashellButton = new MyColorTileButton("Seashell", "#FFF5EE");
    allColorsContentLayout->addWidget(seashellButton, rowCount++, columnCount);
    connect(seashellButton, &QPushButton::clicked, this, [this, seashellButton] () { this->colorTileButtonPicked(seashellButton); });
    
    // ghostwhite
    MyColorTileButton* ghostwhiteButton = new MyColorTileButton("GhostWhite", "#F8F8FF");
    allColorsContentLayout->addWidget(ghostwhiteButton, rowCount++, columnCount);
    connect(ghostwhiteButton, &QPushButton::clicked, this, [this, ghostwhiteButton] () { this->colorTileButtonPicked(ghostwhiteButton); });
    
    // honeydew
    MyColorTileButton* honeydewButton = new MyColorTileButton("Honeydew", "#F0FFF0");
    allColorsContentLayout->addWidget(honeydewButton, rowCount++, columnCount);
    connect(honeydewButton, &QPushButton::clicked, this, [this, honeydewButton] () { this->colorTileButtonPicked(honeydewButton); });
    
    // floralwhite
    MyColorTileButton* floralwhiteButton = new MyColorTileButton("FloralWhite", "#FFFAF0");
    allColorsContentLayout->addWidget(floralwhiteButton, rowCount++, columnCount);
    connect(floralwhiteButton, &QPushButton::clicked, this, [this, floralwhiteButton] () { this->colorTileButtonPicked(floralwhiteButton); });
    
    // azure
    MyColorTileButton* azureButton = new MyColorTileButton("Azure", "#F0FFFF");
    allColorsContentLayout->addWidget(azureButton, rowCount++, columnCount);
    connect(azureButton, &QPushButton::clicked, this, [this, azureButton] () { this->colorTileButtonPicked(azureButton); });
    
    // mintcream
    MyColorTileButton* mintcreamButton = new MyColorTileButton("MintCream", "#F5FFFA");
    allColorsContentLayout->addWidget(mintcreamButton, rowCount++, columnCount);
    connect(mintcreamButton, &QPushButton::clicked, this, [this, mintcreamButton] () { this->colorTileButtonPicked(mintcreamButton); });
    
    // snow
    MyColorTileButton* snowButton = new MyColorTileButton("Snow", "#FFFAFA");
    allColorsContentLayout->addWidget(snowButton, rowCount++, columnCount);
    connect(snowButton, &QPushButton::clicked, this, [this, snowButton] () { this->colorTileButtonPicked(snowButton); });
    
    // ivory
    MyColorTileButton* ivoryButton = new MyColorTileButton("Ivory", "#FFFFF0");
    allColorsContentLayout->addWidget(ivoryButton, rowCount++, columnCount);
    connect(ivoryButton, &QPushButton::clicked, this, [this, ivoryButton] () { this->colorTileButtonPicked(ivoryButton); });
    
    // white
    MyColorTileButton* whiteButton = new MyColorTileButton("White", "#FFFFFF");
    allColorsContentLayout->addWidget(whiteButton, rowCount++, columnCount);
    connect(whiteButton, &QPushButton::clicked, this, [this, whiteButton] () { this->colorTileButtonPicked(whiteButton); });
    
    allColorsWidget->setLayout(allColorsContentLayout);
    allColorsWidget->setContentsMargins(0, 0, 0, 0);
    allColors->setDefaultWidget(allColorsWidget);
    addAction(allColors);
    
    /// frequent colors
    QWidgetAction* frequentColors = new QWidgetAction(this);
    QWidget* frequentColorsWidget = new QWidget(this);
    QGridLayout* frequentColorsContentLayout = new QGridLayout(frequentColorsWidget);
    frequentColorsContentLayout->setSpacing(1);
    
    // label
    frequentColorsContentLayout->addWidget(new MyLabel("Frequent Colors"), frequentColorsContentLayout->rowCount() - 1, 0, 1, 14, Qt::AlignLeft);
    
    frequentColorsContentLayout->addItem(new QSpacerItem(1, 10), frequentColorsContentLayout->rowCount(), 0, 1, 14);
    
    rowCount = 2;
    columnCount = 0;
    
    // black
    MyColorTileButton* balckMainButton = new MyColorTileButton("Black", "#000000");
    frequentColorsContentLayout->addWidget(balckMainButton, rowCount, columnCount++);
    connect(balckMainButton, &QPushButton::clicked, this, [this, balckMainButton] () { this->colorTileButtonPicked(balckMainButton); });
    
    // gray
    MyColorTileButton* grayMainButton = new MyColorTileButton("Gray", "#808080");
    frequentColorsContentLayout->addWidget(grayMainButton, rowCount, columnCount++);
    connect(grayMainButton, &QPushButton::clicked, this, [this, grayMainButton] () { this->colorTileButtonPicked(grayMainButton); });
    
    // brown
    MyColorTileButton* brownMainButton = new MyColorTileButton("Brown", "#A52A2A");
    frequentColorsContentLayout->addWidget(brownMainButton, rowCount, columnCount++);
    connect(brownMainButton, &QPushButton::clicked, this, [this, brownMainButton] () { this->colorTileButtonPicked(brownMainButton); });
    
    // red
    MyColorTileButton* redMainButton = new MyColorTileButton("Red", "#FF0000");
    frequentColorsContentLayout->addWidget(redMainButton, rowCount, columnCount++);
    connect(redMainButton, &QPushButton::clicked, this, [this, redMainButton] () { this->colorTileButtonPicked(redMainButton); });
    
    // orange
    MyColorTileButton* orangeMainButton = new MyColorTileButton("Orange", "#FFA500");
    frequentColorsContentLayout->addWidget(orangeMainButton, rowCount, columnCount++);
    connect(orangeMainButton, &QPushButton::clicked, this, [this, orangeMainButton] () { this->colorTileButtonPicked(orangeMainButton); });
    
    // pink
    MyColorTileButton* pinkMainButton = new MyColorTileButton("Pink", "#FFC0CB");
    frequentColorsContentLayout->addWidget(pinkMainButton, rowCount, columnCount++);
    connect(pinkMainButton, &QPushButton::clicked, this, [this, pinkMainButton] () { this->colorTileButtonPicked(pinkMainButton); });
    
    // purple
    MyColorTileButton* purpleMainButton = new MyColorTileButton("Purple", "#800080");
    frequentColorsContentLayout->addWidget(purpleMainButton, rowCount, columnCount++);
    connect(purpleMainButton, &QPushButton::clicked, this, [this, purpleMainButton] () { this->colorTileButtonPicked(purpleMainButton); });
    
    // magenta
    MyColorTileButton* magentaMainButton = new MyColorTileButton("Magenta", "#FF00FF");
    frequentColorsContentLayout->addWidget(magentaMainButton, rowCount, columnCount++);
    connect(magentaMainButton, &QPushButton::clicked, this, [this, magentaMainButton] () { this->colorTileButtonPicked(magentaMainButton); });
    
    // violet
    MyColorTileButton* violetMainButton = new MyColorTileButton("Violet", "#EE82EE");
    frequentColorsContentLayout->addWidget(violetMainButton, rowCount, columnCount++);
    connect(violetMainButton, &QPushButton::clicked, this, [this, violetMainButton] () { this->colorTileButtonPicked(violetMainButton); });
    
    // blue
    MyColorTileButton* blueMainButton = new MyColorTileButton("Blue", "#0000FF");
    frequentColorsContentLayout->addWidget(blueMainButton, rowCount, columnCount++);
    connect(blueMainButton, &QPushButton::clicked, this, [this, blueMainButton] () { this->colorTileButtonPicked(blueMainButton); });
    
    // cyan
    MyColorTileButton* cyanMainButton = new MyColorTileButton("Cyan", "#00FFFF");
    frequentColorsContentLayout->addWidget(cyanMainButton, rowCount, columnCount++);
    connect(cyanMainButton, &QPushButton::clicked, this, [this, cyanMainButton] () { this->colorTileButtonPicked(cyanMainButton); });
    
    // green
    MyColorTileButton* greenMainButton = new MyColorTileButton("Green", "#008000");
    frequentColorsContentLayout->addWidget(greenMainButton, rowCount, columnCount++);
    connect(greenMainButton, &QPushButton::clicked, this, [this, greenMainButton] () { this->colorTileButtonPicked(greenMainButton); });
    
    // yellow
    MyColorTileButton* yellowMainButton = new MyColorTileButton("Yellow", "#FFFF00");
    frequentColorsContentLayout->addWidget(yellowMainButton, rowCount, columnCount++);
    connect(yellowMainButton, &QPushButton::clicked, this, [this, yellowMainButton] () { this->colorTileButtonPicked(yellowMainButton); });
    
    // white
    MyColorTileButton* whiteMainButton = new MyColorTileButton("White", "#FFFFFF");
    frequentColorsContentLayout->addWidget(whiteMainButton, rowCount, columnCount++);
    connect(whiteMainButton, &QPushButton::clicked, this, [this, whiteMainButton] () { this->colorTileButtonPicked(whiteMainButton); });
    
    frequentColorsWidget->setLayout(frequentColorsContentLayout);
    frequentColorsWidget->setContentsMargins(0, 0, 0, 0);
    frequentColors->setDefaultWidget(frequentColorsWidget);
    addAction(frequentColors);
}

void MyColorPickerMenu::colorTileButtonPicked(MyColorTileButton* colorTileButton) {
    if (colorTileButton) {
        close();
        emit colorChosen(colorTileButton->color(), colorTileButton->value());
    }
}

// MyColorPickerButton

MyColorPickerButton::MyColorPickerButton(MainWindow* mw) {
    _mw = mw;
    setArrowType(Qt::NoArrow);
    setPopupMode(QToolButton::InstantPopup);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setStyleSheet("QToolButton { border: 1px solid LightSlateGray; border-radius: 6px; background-color: white} QToolButton::menu-indicator { image: none; }" );
    setFixedWidth(30);
    connect(this, SIGNAL(clicked()), this, SLOT(setColorMenu()));
}

void MyColorPickerButton::setBackgroundColor(const QString& color)  {
    if (!color.isEmpty())
        setStyleSheet("QToolButton { border: 1px solid LightSlateGray; border-radius: 5px; background-color:" + color + " } QToolButton::menu-indicator { image: none; } " );
}

void MyColorPickerButton::setColorMenu() {
    if (_mw && _mw->isSetFeatureMenu() && _mw->getFeatureMenu()->getColorPickerMenu()) {
        setMenu(_mw->getFeatureMenu()->getColorPickerMenu());
        _mw->getFeatureMenu()->getColorPickerMenu()->disconnect();
        connect(_mw->getFeatureMenu()->getColorPickerMenu(), SIGNAL(colorChosen(const QString&, const QString&)), this, SIGNAL(colorChosen(const QString&, const QString&)));
        showMenu();
    }
}

// MyAddOrRemoveButton

MyAddOrRemoveButton::MyAddOrRemoveButton(QWidget* parent) : QPushButton(parent) {
    setFixedWidth(50.0);
}

void MyAddOrRemoveButton::enableRemove() {
    setText("-");
}

void MyAddOrRemoveButton::resetValues() {
    setText("+");
}

// MyAddOrRemoveReactionBoundingBoxButton

MyAddOrRemoveReactionBoundingBoxButton::MyAddOrRemoveReactionBoundingBoxButton(MainWindow* mw, QWidget* parent) : MyAddOrRemoveButton(parent) {
    connect(this, &QPushButton::clicked, this, [this] () {
        if (text() == "-")
        emit removeBoundingBox();
    });
    
    addingMenu = new QMenu(this);
    
    QWidgetAction* addBoxAction = new QWidgetAction(addingMenu);
    QWidget* addBoxWidget = new QWidget(addingMenu);
    QGridLayout* addBoxContentLayout = new QGridLayout(addBoxWidget);
    
    // position
    addBoxContentLayout->addWidget(new MyLabel("Position") , addBoxContentLayout->rowCount(), 0);
    positionDualSpinBox.setType("Position");
    addBoxContentLayout->addWidget(&positionDualSpinBox, addBoxContentLayout->rowCount() - 1, 1);
    
    // dimensions
    addBoxContentLayout->addWidget(new MyLabel("Dimensions") , addBoxContentLayout->rowCount(), 0);
    dimensionsDualSpinBox.setType("Dimensions");
    addBoxContentLayout->addWidget(&dimensionsDualSpinBox, addBoxContentLayout->rowCount() - 1, 1);
    
    fromCurveExtentsButton.setText("From Curve Extents");
    addBoxContentLayout->addWidget(&fromCurveExtentsButton, addBoxContentLayout->rowCount(), 0);
    
    // set ranges
    setBoxRanges(mw);
    
    QDialogButtonBox* addBoxButtons = new QDialogButtonBox(Qt::Horizontal, addBoxWidget);
    addBoxButtons->addButton(QString("Add"), QDialogButtonBox::AcceptRole);
    addBoxContentLayout->addWidget(addBoxButtons, addBoxContentLayout->rowCount() - 1, 1);
    connect(addBoxButtons, &QDialogButtonBox::accepted, this, [this] () {
        this->addingMenu->close();
        emit boundingBoxFeaturesChosen(this->positionDualSpinBox.first(), this->positionDualSpinBox.second(), this->dimensionsDualSpinBox.first(), this->dimensionsDualSpinBox.second());
    });
    
    addBoxWidget->setLayout(addBoxContentLayout);
    addBoxWidget->setContentsMargins(0, 0, 0, 0);
    addBoxAction->setDefaultWidget(addBoxWidget);
    addingMenu->addAction(addBoxAction);
     
    resetValues();
}

void MyAddOrRemoveReactionBoundingBoxButton::enableFromCurveExtents(LBox* extentBox) {
    if (extentBox) {
        fromCurveExtentsButton.setEnabled(true);
        connect(&fromCurveExtentsButton, &QPushButton::clicked, this, [this, extentBox] () {
            this->positionDualSpinBox.setCurrentValue(ne_bb_getX(extentBox), ne_bb_getY(extentBox));
            this->dimensionsDualSpinBox.setCurrentValue(std::max(5.0, ne_bb_getWidth(extentBox)), std::max(5.0, ne_bb_getHeight(extentBox)));
        });
    }
}

void MyAddOrRemoveReactionBoundingBoxButton::initialize(LBox* box) {
    if (box) {
        positionDualSpinBox.setCurrentValue(ne_bb_getX(box), ne_bb_getY(box));
        dimensionsDualSpinBox.setCurrentValue(ne_bb_getWidth(box), ne_bb_getHeight(box));
    }
}

void MyAddOrRemoveReactionBoundingBoxButton::enableRemove() {
    setText("-");
    setMenu(NULL);
}

void MyAddOrRemoveReactionBoundingBoxButton::setBoxRanges(MainWindow* mw) {
    positionDualSpinBox.setRange(mw);
    dimensionsDualSpinBox.setRange(mw);
}

void MyAddOrRemoveReactionBoundingBoxButton::resetValues() {
    setText("+");
    fromCurveExtentsButton.setEnabled(false);
    positionDualSpinBox.resetValues();
    dimensionsDualSpinBox.resetValues();
    setMenu(addingMenu);
}

// MyAddRemoveButtons

MyAddRemoveButtons::MyAddRemoveButtons(QWidget* parent) : QDialogButtonBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setOrientation(Qt::Horizontal);
    // add button
    addPushButton = addButton(QString("+"), QDialogButtonBox::YesRole);
    addingMenu = new QMenu(addPushButton);
    addPushButton->setMenu(addingMenu);
    
    // remove button
    removePushButton = addButton(QString("-"), QDialogButtonBox::NoRole);
    removingMenu = new QMenu(removePushButton);
    removePushButton->setMenu(removingMenu);
}

MyAddRemoveTextButtons::MyAddRemoveTextButtons(QWidget* parent) : MyAddRemoveButtons(parent) {
    if (addingMenu) {
        QWidgetAction* addTextAction = new QWidgetAction(addingMenu);
        plainTextWidget = new MyPlainTextWidget();
        connect(plainTextWidget, &MyPlainTextWidget::plainTextChanged, this, [this] () {
            this->addingMenu->close();
            emit textChosen(plainTextWidget->plainText());
        });
        addTextAction->setDefaultWidget(plainTextWidget);
        addingMenu->addAction(addTextAction);
    }
}

void MyAddRemoveTextButtons::enableAddButton(NGraphicalObject* gO) {
    if (addPushButton)
        addPushButton->setEnabled(true);
    
    if (gO && plainTextWidget)
        plainTextWidget->enable(gO);
}

void MyAddRemoveTextButtons::enableRemoveButton(const unsigned int& numberOfElements) {
    if (removePushButton) {
        removePushButton->setEnabled(true);
        for (int i = 0; i < numberOfElements; ++i)
            connect(removingMenu->addAction(" Text " + QString::number(i + 1)), &QAction::triggered, this, [this, i] () { emit removeItemFeaturesChosen(i); });
    }
}

void MyAddRemoveTextButtons::resetValues() {
    if (removePushButton)
        removePushButton->setEnabled(false);
    
    if (addPushButton)
        addPushButton->setEnabled(false);
    
    if (removingMenu)
        removingMenu->clear();
    
    if (plainTextWidget)
        plainTextWidget->resetValues();
}

// MyAddRemoveGeometricShapeButtons

MyAddRemoveGeometricShapeButtons::MyAddRemoveGeometricShapeButtons(QWidget* parent) : MyAddRemoveButtons(parent) {
    if (addPushButton) {
        connect(addingMenu->addAction("Rectangle"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Rectangle"); });
        connect(addingMenu->addAction("Ellipse"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Ellipse"); });
        connect(addingMenu->addAction("Image"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Image"); });
        connect(addingMenu->addAction("RenderCurve"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("RenderCurve"); });
        
        QMenu* polygonsMenu = addingMenu->addMenu("Polygons");
        connect(polygonsMenu->addAction("Triangle"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Triangle"); });
        connect(polygonsMenu->addAction("Diamond"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Diamond"); });
        connect(polygonsMenu->addAction("Pentagon"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Pentagon"); });
        connect(polygonsMenu->addAction("Hexagon"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Hexagon"); });
        connect(polygonsMenu->addAction("Octagon"), &QAction::triggered, this, [this] () { emit geometricShapeChosen("Octagon"); });
    }
}

void MyAddRemoveGeometricShapeButtons::enableRemoveButton(VRenderGroup* group) {
    if (group && removePushButton) {
        removePushButton->setEnabled(true);
        QString shape;
        for (int i = 0; i < ne_grp_getNumGeometricShapes(group); ++i) {
            switch (ne_gs_getShape(ne_grp_getGeometricShape(group, i))) {
                case 0:
                    shape = "Image";
                    break;
                    
                case 1:
                    shape = "RenderCurve";
                    break;
                    
                //case 2:
                    //gSFMenuElement = new TextGeometricShapeFeatureMenu(this, _mw);
                    //break;
                    
                case 3:
                    shape = "Rectangle";
                    break;
                    
                case 4:
                    shape = "Ellipse";
                    break;
                    
                case 5:
                    shape = "Polygon";
                    break;
                    
                default:
                    break;
            }
            
            connect(removingMenu->addAction(QString::number(i + 1) + ": " + shape), &QAction::triggered, this, [this, i] () { emit removeItemFeaturesChosen(i); });
        }
    }
}

void MyAddRemoveGeometricShapeButtons::enableAddButton() {
    if (addPushButton)
        addPushButton->setEnabled(true);
}

void MyAddRemoveGeometricShapeButtons::resetValues(const bool& disableAddButton) {
    if (removePushButton)
        removePushButton->setEnabled(false);
    
    if (disableAddButton && addPushButton)
        addPushButton->setEnabled(false);
    
    if (removingMenu)
        removingMenu->clear();
}

// MyAddRemoveSegmentButtons

MyAddRemoveSegmentButtons::MyAddRemoveSegmentButtons(QWidget* parent) : MyAddRemoveButtons(parent) {
    
}

void MyAddRemoveSegmentButtons::enableAddButton(MainWindow* mw, const unsigned int& numberOfElements) {
    if (addPushButton) {
        addPushButton->setEnabled(true);
        /// point feature menu
        QMenu* addPointMenu = addingMenu->addMenu("Add Point");
        QWidgetAction* addPointAction = new QWidgetAction(addPointMenu);
        QWidget* addPointWidget = new QWidget(addPointMenu);
        QGridLayout* addPointContentLayout = new QGridLayout(addPointWidget);
        
        addPointContentLayout->addWidget(new MyLabel("Place"), addPointContentLayout->rowCount(), 0);
        QComboBox* pointPlaceComboBox = new QComboBox(addPointWidget);
        for (int i = 1; i <= numberOfElements; ++i)
            pointPlaceComboBox->addItem(QString::number(i));
        pointPlaceComboBox->addItem(QString("Last"));
        pointPlaceComboBox->setCurrentIndex(numberOfElements);
        addPointContentLayout->addWidget(pointPlaceComboBox, addPointContentLayout->rowCount() - 1, 1);
        
        // start
        addPointContentLayout->addWidget(new MyLabel("Start") , addPointContentLayout->rowCount(), 0);
        MyDualSpinBox* pointStartDualSpinBox = new MyDualSpinBox("Position");
        pointStartDualSpinBox->setRange(mw);
        addPointContentLayout->addWidget(pointStartDualSpinBox, addPointContentLayout->rowCount() - 1, 1);
        
        // end
        addPointContentLayout->addWidget(new MyLabel("End") , addPointContentLayout->rowCount(), 0);
        MyDualSpinBox* pointEndDualSpinBox = new MyDualSpinBox("Position");
        pointEndDualSpinBox->setRange(mw);
        addPointContentLayout->addWidget(pointEndDualSpinBox, addPointContentLayout->rowCount() - 1, 1);
        
        QDialogButtonBox* addPointButtons = new QDialogButtonBox(Qt::Horizontal, addPointWidget);
        addPointButtons->addButton(QString("Add"), QDialogButtonBox::AcceptRole);
        addPointContentLayout->addWidget(addPointButtons, addPointContentLayout->rowCount(), 1);
        connect(addPointButtons, &QDialogButtonBox::accepted, this, [this, pointPlaceComboBox, pointStartDualSpinBox, pointEndDualSpinBox] () {
            this->addingMenu->close();
            emit addItemFeaturesChosen(pointPlaceComboBox->currentIndex(), pointStartDualSpinBox->x(), pointStartDualSpinBox->y(), pointEndDualSpinBox->x(), pointEndDualSpinBox->y());
        });
        
        addPointWidget->setLayout(addPointContentLayout);
        addPointWidget->setContentsMargins(0, 0, 0, 0);
        addPointAction->setDefaultWidget(addPointWidget);
        addPointMenu->addAction(addPointAction);
        
        /// cubic bezier feature menu
        QMenu* addCubicBezierMenu = addingMenu->addMenu("Add Cubic Bezier");
        QWidgetAction* addCubicBezierAction = new QWidgetAction(addCubicBezierMenu);
        QWidget* addCubicBezierWidget = new QWidget(addCubicBezierMenu);
        QGridLayout* addCubicBezierContentLayout = new QGridLayout(addCubicBezierWidget);
        
        addCubicBezierContentLayout->addWidget(new MyLabel("Place"), addCubicBezierContentLayout->rowCount(), 0);
        QComboBox* cubicBezierPlaceComboBox = new QComboBox(addCubicBezierWidget);
        for (int i = 1; i <= numberOfElements; ++i)
            cubicBezierPlaceComboBox->addItem(QString::number(i));
        cubicBezierPlaceComboBox->addItem(QString("Last"));
        cubicBezierPlaceComboBox->setCurrentIndex(numberOfElements);
        addCubicBezierContentLayout->addWidget(cubicBezierPlaceComboBox, addCubicBezierContentLayout->rowCount() - 1, 1);
        
        // start
        addCubicBezierContentLayout->addWidget(new MyLabel("Start") , addCubicBezierContentLayout->rowCount(), 0);
        MyDualSpinBox* cubicBezierStartDualSpinBox = new MyDualSpinBox("Position");
        cubicBezierStartDualSpinBox->setRange(mw);
        addCubicBezierContentLayout->addWidget(cubicBezierStartDualSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1);
        
        // end
        addCubicBezierContentLayout->addWidget(new MyLabel("End") , addCubicBezierContentLayout->rowCount(), 0);
        MyDualSpinBox* cubicBezierEndDualSpinBox = new MyDualSpinBox("Position");
        cubicBezierEndDualSpinBox->setRange(mw);
        addCubicBezierContentLayout->addWidget(cubicBezierEndDualSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1);
        
        // basepoint1
        addCubicBezierContentLayout->addWidget(new MyLabel("BasePoint1") , addCubicBezierContentLayout->rowCount(), 0);
        MyDualSpinBox* cubicBezierBasePoint1DualSpinBox = new MyDualSpinBox("Position");
        cubicBezierBasePoint1DualSpinBox->setRange(mw);
        addCubicBezierContentLayout->addWidget(cubicBezierBasePoint1DualSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1);
        
        // basepoint2
        addCubicBezierContentLayout->addWidget(new MyLabel("BasePoint2") , addCubicBezierContentLayout->rowCount(), 0);
        MyDualSpinBox* cubicBezierBasePoint2DualSpinBox = new MyDualSpinBox("Position");
        cubicBezierBasePoint2DualSpinBox->setRange(mw);
        addCubicBezierContentLayout->addWidget(cubicBezierBasePoint2DualSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1);
        
        QDialogButtonBox* addCubicBezierButtons = new QDialogButtonBox(Qt::Horizontal, addCubicBezierWidget);
        addCubicBezierButtons->addButton(QString("Add"), QDialogButtonBox::AcceptRole);
        addCubicBezierContentLayout->addWidget(addCubicBezierButtons, addCubicBezierContentLayout->rowCount(), 1);
        connect(addCubicBezierButtons, &QDialogButtonBox::accepted, this, [this, cubicBezierPlaceComboBox, cubicBezierStartDualSpinBox, cubicBezierEndDualSpinBox,  cubicBezierBasePoint1DualSpinBox, cubicBezierBasePoint2DualSpinBox] () {
            this->addingMenu->close();
            emit addItemFeaturesChosen(cubicBezierPlaceComboBox->currentIndex(), cubicBezierStartDualSpinBox->x(), cubicBezierStartDualSpinBox->y(), cubicBezierEndDualSpinBox->x(), cubicBezierEndDualSpinBox->y(), cubicBezierBasePoint1DualSpinBox->x(), cubicBezierBasePoint1DualSpinBox->y(), cubicBezierBasePoint2DualSpinBox->x(), cubicBezierBasePoint2DualSpinBox->y()); });
        
        addCubicBezierWidget->setLayout(addCubicBezierContentLayout);
        addCubicBezierWidget->setContentsMargins(0, 0, 0, 0);
        addCubicBezierAction->setDefaultWidget(addCubicBezierWidget);
        addCubicBezierMenu->addAction(addCubicBezierAction);
    }
}

void MyAddRemoveSegmentButtons::enableRemoveButton(const unsigned int& numberOfElements) {
    if (removePushButton) {
        removePushButton->setEnabled(true);
        for (int i = 0; i < numberOfElements; ++i)
            connect(removingMenu->addAction("Segment " + QString::number(i + 1)), &QAction::triggered, this, [this, i] () { emit removeItemFeaturesChosen(i); });
    }
}

void MyAddRemoveSegmentButtons::resetValues() {
    if (addPushButton)
        addPushButton->setEnabled(false);
    
    if (removePushButton)
        removePushButton->setEnabled(false);
    
    if (removingMenu)
        removingMenu->clear();
    
    if (addingMenu)
        addingMenu->clear();
}

// MyAddRemoveVertexButtons

MyAddRemoveVertexButtons::MyAddRemoveVertexButtons(QWidget* parent) : MyAddRemoveButtons(parent) {
    
}

void MyAddRemoveVertexButtons::enableAddButton(MainWindow* mw, LBox* bBox, const unsigned int& numberOfElements) {
    if (addPushButton) {
        /// point feature menu
        QMenu* addPointMenu = addingMenu->addMenu("Add Point");
        QWidgetAction* addPointAction = new QWidgetAction(addPointMenu);
        QWidget* addPointWidget = new QWidget(addPointMenu);
        QGridLayout* addPointContentLayout = new QGridLayout(addPointWidget);
        
        addPointContentLayout->addWidget(new MyLabel("Place"), addPointContentLayout->rowCount(), 0);
        QComboBox* pointPlaceComboBox = new QComboBox(addPointWidget);
        for (int i = 1; i <= numberOfElements; ++i)
            pointPlaceComboBox->addItem(QString::number(i));
        pointPlaceComboBox->addItem(QString("Last"));
        pointPlaceComboBox->setCurrentIndex(numberOfElements);
        addPointContentLayout->addWidget(pointPlaceComboBox, addPointContentLayout->rowCount() - 1, 1, 1, 2);
        
        // rpoint
        // x
        MyRelAbsSpinBox* pointXRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            pointXRelAbsSpinBox->setRange(mw->getScene()->sceneRect().x() - ne_bb_getX(bBox), mw->getScene()->sceneRect().x() + mw->getScene()->sceneRect().width() - ne_bb_getX(bBox));
        addPointContentLayout->addWidget(new MyLabel("X"), addPointContentLayout->rowCount(), 0);
        addPointContentLayout->addWidget(pointXRelAbsSpinBox, addPointContentLayout->rowCount() - 1, 1, 1, 2);
        // y
        MyRelAbsSpinBox* pointYRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            pointYRelAbsSpinBox->setRange(mw->getScene()->sceneRect().y() - ne_bb_getY(bBox), mw->getScene()->sceneRect().y() + mw->getScene()->sceneRect().height() - ne_bb_getY(bBox));
        addPointContentLayout->addWidget(new MyLabel("Y"), addPointContentLayout->rowCount(), 0);
        addPointContentLayout->addWidget(pointYRelAbsSpinBox, addPointContentLayout->rowCount() - 1, 1, 1, 2);
        
        QDialogButtonBox* addPointButtons = new QDialogButtonBox(Qt::Horizontal, addPointWidget);
        addPointButtons->addButton(QString("Add"), QDialogButtonBox::AcceptRole);
        addPointContentLayout->addWidget(addPointButtons, addPointContentLayout->rowCount(), 2);
        connect(addPointButtons, &QDialogButtonBox::accepted, this, [this, pointPlaceComboBox, pointXRelAbsSpinBox, pointYRelAbsSpinBox] () {
            this->addingMenu->close();
            emit addItemFeaturesChosen(pointPlaceComboBox->currentIndex(), pointXRelAbsSpinBox->absoluteValue(), pointXRelAbsSpinBox->relativeValue(), pointYRelAbsSpinBox->absoluteValue(), pointYRelAbsSpinBox->relativeValue());
        });
        
        addPointWidget->setLayout(addPointContentLayout);
        addPointWidget->setContentsMargins(0, 0, 0, 0);
        addPointAction->setDefaultWidget(addPointWidget);
        addPointMenu->addAction(addPointAction);
        
        /// cubic bezier feature menu
        QMenu* addCubicBezierMenu = addingMenu->addMenu("Add Cubic Bezier");
        QWidgetAction* addCubicBezierAction = new QWidgetAction(addCubicBezierMenu);
        QWidget* addCubicBezierWidget = new QWidget(addCubicBezierMenu);
        QGridLayout* addCubicBezierContentLayout = new QGridLayout(addCubicBezierWidget);
        
        addCubicBezierContentLayout->addWidget(new MyLabel("Place"), addCubicBezierContentLayout->rowCount(), 0);
        QComboBox* cubicBezierPlaceComboBox = new QComboBox(addCubicBezierWidget);
        for (int i = 1; i <= numberOfElements; ++i)
            cubicBezierPlaceComboBox->addItem(QString::number(i));
        cubicBezierPlaceComboBox->addItem(QString("Last"));
        cubicBezierPlaceComboBox->setCurrentIndex(numberOfElements);
        addCubicBezierContentLayout->addWidget(cubicBezierPlaceComboBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        
        // rpoint
        // x
        MyRelAbsSpinBox* cubicBezierXRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            cubicBezierXRelAbsSpinBox->setRange(mw->getScene()->sceneRect().x() - ne_bb_getX(bBox), mw->getScene()->sceneRect().x() + mw->getScene()->sceneRect().width() - ne_bb_getX(bBox));
        addCubicBezierContentLayout->addWidget(new MyLabel("X"), addCubicBezierContentLayout->rowCount(), 0);
        addCubicBezierContentLayout->addWidget(cubicBezierXRelAbsSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        // y
        MyRelAbsSpinBox* cubicBezierYRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            cubicBezierYRelAbsSpinBox->setRange(mw->getScene()->sceneRect().y() - ne_bb_getY(bBox), mw->getScene()->sceneRect().y() + mw->getScene()->sceneRect().height() - ne_bb_getY(bBox));
        addCubicBezierContentLayout->addWidget(new MyLabel("Y"), addCubicBezierContentLayout->rowCount(), 0);
        addCubicBezierContentLayout->addWidget(cubicBezierYRelAbsSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        
        // basepoint1
        // x
        MyRelAbsSpinBox* cubicBezierC1XRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            cubicBezierC1XRelAbsSpinBox->setRange(mw->getScene()->sceneRect().x() - ne_bb_getX(bBox), mw->getScene()->sceneRect().x() + mw->getScene()->sceneRect().width() - ne_bb_getX(bBox));
        addCubicBezierContentLayout->addWidget(new MyLabel("C1X"), addCubicBezierContentLayout->rowCount(), 0);
        addCubicBezierContentLayout->addWidget(cubicBezierC1XRelAbsSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        // y
        MyRelAbsSpinBox* cubicBezierC1YRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            cubicBezierC1YRelAbsSpinBox->setRange(mw->getScene()->sceneRect().y() - ne_bb_getY(bBox), mw->getScene()->sceneRect().y() + mw->getScene()->sceneRect().height() - ne_bb_getY(bBox));
        addCubicBezierContentLayout->addWidget(new MyLabel("C1Y"), addCubicBezierContentLayout->rowCount(), 0);
        addCubicBezierContentLayout->addWidget(cubicBezierC1YRelAbsSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        
        // basepoint2
        // x
        MyRelAbsSpinBox* cubicBezierC2XRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            cubicBezierC2XRelAbsSpinBox->setRange(mw->getScene()->sceneRect().x() - ne_bb_getX(bBox), mw->getScene()->sceneRect().x() + mw->getScene()->sceneRect().width() - ne_bb_getX(bBox));
        addCubicBezierContentLayout->addWidget(new MyLabel("C2X"), addCubicBezierContentLayout->rowCount(), 0);
        addCubicBezierContentLayout->addWidget(cubicBezierC2XRelAbsSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        // y
        MyRelAbsSpinBox* cubicBezierC2YRelAbsSpinBox = new MyRelAbsSpinBox();
        if (mw && bBox)
            cubicBezierC2YRelAbsSpinBox->setRange(mw->getScene()->sceneRect().y() - ne_bb_getY(bBox), mw->getScene()->sceneRect().y() + mw->getScene()->sceneRect().height() - ne_bb_getY(bBox));
        addCubicBezierContentLayout->addWidget(new MyLabel("C2Y"), addCubicBezierContentLayout->rowCount(), 0);
        addCubicBezierContentLayout->addWidget(cubicBezierC2YRelAbsSpinBox, addCubicBezierContentLayout->rowCount() - 1, 1, 1, 2);
        
        QDialogButtonBox* addCubicBezierButtons = new QDialogButtonBox(Qt::Horizontal, addCubicBezierWidget);
        addCubicBezierButtons->addButton(QString("Add"), QDialogButtonBox::AcceptRole);
        addCubicBezierContentLayout->addWidget(addCubicBezierButtons, addCubicBezierContentLayout->rowCount(), 2);
        connect(addCubicBezierButtons, &QDialogButtonBox::accepted, this, [this, cubicBezierPlaceComboBox, cubicBezierXRelAbsSpinBox, cubicBezierYRelAbsSpinBox,  cubicBezierC1XRelAbsSpinBox, cubicBezierC1YRelAbsSpinBox, cubicBezierC2XRelAbsSpinBox, cubicBezierC2YRelAbsSpinBox] () {
            this->addingMenu->close();
            emit addItemFeaturesChosen(cubicBezierPlaceComboBox->currentIndex(), cubicBezierXRelAbsSpinBox->absoluteValue(), cubicBezierXRelAbsSpinBox->relativeValue(), cubicBezierYRelAbsSpinBox->absoluteValue(), cubicBezierYRelAbsSpinBox->relativeValue(), cubicBezierC1XRelAbsSpinBox->absoluteValue(), cubicBezierC1XRelAbsSpinBox->relativeValue(), cubicBezierC1YRelAbsSpinBox->absoluteValue(), cubicBezierC1YRelAbsSpinBox->relativeValue(), cubicBezierC2XRelAbsSpinBox->absoluteValue(), cubicBezierC2XRelAbsSpinBox->relativeValue(), cubicBezierC2YRelAbsSpinBox->absoluteValue(), cubicBezierC2YRelAbsSpinBox->relativeValue()); });
        
        addCubicBezierWidget->setLayout(addCubicBezierContentLayout);
        addCubicBezierWidget->setContentsMargins(0, 0, 0, 0);
        addCubicBezierAction->setDefaultWidget(addCubicBezierWidget);
        addCubicBezierMenu->addAction(addCubicBezierAction);
    }
}

void MyAddRemoveVertexButtons::enableRemoveButton(const unsigned int& numberOfElements) {
    if (removePushButton) {
        removePushButton->setEnabled(true);
        for (int i = 0; i < numberOfElements; ++i)
            connect(removingMenu->addAction("Vertex " + QString::number(i + 1)), &QAction::triggered, this, [this, i] () { emit removeItemFeaturesChosen(i); });
    }
}

void MyAddRemoveVertexButtons::resetValues() {
    if (removePushButton)
        removePushButton->setEnabled(false);
    
    if (removingMenu)
        removingMenu->clear();
    
    if (addingMenu)
        addingMenu->clear();
}

// MyStandardItem

MyStandardItem::MyStandardItem(const QString& text, const qreal& fontsize, const bool& isbold, const QColor& color) {
    //QFont font("Open Sans", fontsize);
    QFont font;
    font.setPointSize(fontsize);
    font.setBold(isbold);
    
    setEditable(false);
    setSelectable(false);
    setForeground(color);
    setFont(font);
    setText(text);
}

// MyGroupBox

MyGroupBox::MyGroupBox(QWidget* parent) : QGroupBox(parent) {
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setStyleSheet("QGroupBox { background-color: white; border: no-border;}");
    setContentsMargins(0, 0, 0, 0);
}

// MyTreeView

MyTreeView::MyTreeView(QWidget* parent) : QTreeView(parent) {
    setHeaderHidden(true);
    setStyleSheet("QTreeView { background-color: white; border: no-border;}" "QTreeView::item:selected { background-color: white; border: no-border;}" "QTreeView::item:hover { background-color: white; border: no-border;}");
    setContentsMargins(0, 0, 0, 0);
    //setAnimated(true);
    
    treeModel = new QStandardItemModel(this);
    setModel(treeModel);
    
    connect(this, QOverload<const QModelIndex&>::of(&QTreeView::expanded), this, [this] (const QModelIndex& index) {
        if (treeModel) {
            std::vector<QStandardItem*> familyItems;
            std::list<QStandardItem*> items;
            QStandardItem* root = treeModel->invisibleRootItem();;
            QStandardItem* item = treeModel->itemFromIndex(index);
            int n = 0;
            int m = 0;
            
            familyItems.push_back(item);
            while (item->parent()) {
                item = item->parent();
                familyItems.push_back(item);
            }
            
            for (int i = 0; i < root->rowCount(); ++i) {
                items.push_back(root->child(i));
                
                while (!items.empty()) {
                    n = items.size();
                    
                    while (n > 0) {
                        item = items.front();
                        items.pop_front();
                        
                        m = 0;
                        for (m = 0; m < familyItems.size(); ++m) {
                            if (item->text() == familyItems.at(m)->text())
                                break;
                        }
                        
                        if (m == familyItems.size())
                            this->collapse(treeModel->indexFromItem(item));
                        
                        //if (!item->hasChildren())
                            //break;
                        
                        for (int j = 0; j < item->rowCount(); ++j)
                            items.push_back(item->child(j));
                        
                        n--;
                    }
                }
            }
            
            this->scrollTo(index, QAbstractItemView::PositionAtTop);
        }
    });
}

void MyTreeView::addBranchWidget(QWidget* branchWidget, const QString& branchTitle, const QString& rootTitle) {
    MyStandardItem* branch = new MyStandardItem(branchTitle, 12.0, true);
    if (treeModel->findItems(rootTitle).empty())
        treeModel->invisibleRootItem()->appendRow(branch);
    else
        treeModel->findItems(rootTitle).first()->appendRow(branch);
    
    if (branchWidget) {
        MyStandardItem* branchContent = new MyStandardItem();
        branch->appendRow(branchContent);
        setIndexWidget(branchContent->index(), branchWidget);
        _branches.push_back(std::pair<MyStandardItem*, MyStandardItem*>(branch, branchContent));
    }
    else
        _branches.push_back(std::pair<MyStandardItem*, MyStandardItem*>(branch, NULL));
}

void MyTreeView::clearModel() {
    for (constBranchIt bIt = BranchesBegin(); bIt != BranchesEnd(); ++bIt) {
        if ((*bIt).second) {
            (*bIt).second->removeRows(0, (*bIt).second->rowCount());
            delete (*bIt).second;
        }
        //(*bIt).first->removeRows(0, (*bIt).first->rowCount());
        delete (*bIt).first;
    }
    _branches.clear();
    
    if (treeModel) {
        //treeModel->clear();
        delete treeModel;
    }
}

void MyTreeView::removeBranches(const QString& rootTitle, const unsigned int& staticbranches) {
    QList<QStandardItem *> roots;
    if (!rootTitle.isEmpty())
        roots = treeModel->findItems(rootTitle);
    else
        roots.push_back(treeModel->invisibleRootItem());
    for (int i = 0; i < roots.size(); ++i) {
        while (roots.at(i)->rowCount() > staticbranches) {
            for (constBranchIt bIt = BranchesBegin(); bIt != BranchesEnd(); ++bIt) {
                if ((*bIt).first->text() == roots.at(i)->child(staticbranches)->text()) {
                    if ((*bIt).second) {
                        (*bIt).second->clearData();
                        (*bIt).second->removeRows(0, (*bIt).second->rowCount());
                        delete (*bIt).second;
                    }
                    (*bIt).first->clearData();
                    (*bIt).first->removeRows(0, (*bIt).first->rowCount());
                    delete (*bIt).first;
                    _branches.erase(bIt);
                    roots.at(i)->removeRow(staticbranches);
                    break;
                }
            }
        }
    }
}

// MyTabWidget

MyTabWidget::MyTabWidget(QWidget* parent) : QTabWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QTabWidget::pane { border: 1px solid lightgray; border-radius: 5px; top:-1px; background: white; } QTabBar::tab { width: 65px; background: rgb(230, 230, 230); border: 1px solid lightgray; border-top-left-radius: 5px; border-top-right-radius: 5px; padding: 10px;} QTabBar::tab:selected { background: white; margin-bottom: -1px; }");
    setContentsMargins(0, 0, 0, 0);
}

// Feature Menu

FeatureMenu::FeatureMenu(QWidget* parent, MainWindow* mw) {
    _featureMenuElements.clear();
    FeatureMenuElement* fMElement;
    
    // add species feature menu
    fMElement = new SpeciesFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add compartment feature menu
    fMElement = new CompartmentFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add reaction feature menu
    fMElement = new ReactionFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add species reference feature menu
    fMElement = new SpeciesReferenceFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add line ending feature menu
    fMElement = new LineEndingFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
#if TELLURIUM_INCLUDED
    // add model feature menu
    fMElement = new ModelFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
#endif
    
    // construct color picker menu
    _colorPickerMenu = new MyColorPickerMenu();
}

const FeatureMenu::fMenuElementVec& FeatureMenu::getFeatureMenuElements() const {
    return _featureMenuElements;
}

const size_t FeatureMenu::getNumFeatureMenuElements() const {
    return _featureMenuElements.size();
}

void FeatureMenu::showFeatureMenu(GraphicalSpecies* gS) {
    ((SpeciesFeatureMenu*)(_featureMenuElements.at(0)))->showInfo(gS);
}

void FeatureMenu::showFeatureMenu(GraphicalCompartment* gC) {
    ((CompartmentFeatureMenu*)(_featureMenuElements.at(1)))->showInfo(gC);
}

void FeatureMenu::showFeatureMenu(GraphicalReaction* gR) {
    ((ReactionFeatureMenu*)(_featureMenuElements.at(2)))->showInfo(gR);
}

void FeatureMenu::showFeatureMenu(GraphicalSReference* gSR) {
    ((SpeciesReferenceFeatureMenu*)(_featureMenuElements.at(3)))->showInfo(gSR);
}

void FeatureMenu::showFeatureMenu(GraphicalLineEnding* gLE, GraphicalSReference* gSR, QString head) {
    ((LineEndingFeatureMenu*)(_featureMenuElements.at(4)))->showInfo(gLE, gSR, head);
}

#if TELLURIUM_INCLUDED
void FeatureMenu::showFeatureMenu() {
    ((ModelFeatureMenu*)(_featureMenuElements.at(5)))->showInfo();
}
#endif

void FeatureMenu::hideFeatureMenu() {
    for (constFMenuElementIt fMEIt = fMenuElementsBegin(); fMEIt != fMenuElementsEnd(); ++fMEIt)
        (*fMEIt)->hideInfo();
}

QMenu* FeatureMenu::getColorPickerMenu() {
    return _colorPickerMenu;
}

// Feature Menu Element

FeatureMenuElement::FeatureMenuElement(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; font: bold; border: 1px white; border-radius: 5px; margin-top: 1ex; }" "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0px 5px 0px 5px; }");
    setContentsMargins(0, 0, 0, 0);
    QFont headlineFont = titleLabel.font();
    headlineFont.setBold(true);
    headlineFont.setCapitalization(QFont::SmallCaps);
    headlineFont.setPointSize(24);
    titleLabel.setFont(headlineFont);
    qLayout.setAlignment(Qt::AlignTop);
    qLayout.setContentsMargins(10, 20, 10, 10);
    setLayout(&qLayout);
    qLayout.addWidget(&titleLabel, qLayout.rowCount() - 1, 0);
}

void FeatureMenuElement::hideInfo() {
    hide();
    
    // tabs
    if (tabMenu)
        tabMenu->setCurrentIndex(0);
}

// Model Feature Menu

ModelFeatureMenu::ModelFeatureMenu(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    tabMenu = NULL;
    simulationBranch = NULL;
    setFixedSize(QSize(350, maximumHeight()));
    titleLabel.setText("Model");
    tabMenu = new MyTabWidget(this);
    
    /// simulation
    simulationBranch = new MyGroupBox(tabMenu);
    // start time
    simulationContentLayout.addWidget(new MyLabel("Start Time:"), simulationContentLayout.rowCount() - 1, 0);
    startTimeSpinBox.setMinimum(0.0);
    startTimeSpinBox.setFixedWidth(75.0);
    simulationContentLayout.addWidget(&startTimeSpinBox, simulationContentLayout.rowCount() - 1, 2);
    // end time
    simulationContentLayout.addWidget(new MyLabel("End Time:"), simulationContentLayout.rowCount(), 0);
    endTimeSpinBox.setMinimum(1.0);
    endTimeSpinBox.setFixedWidth(75.0);
    simulationContentLayout.addWidget(&endTimeSpinBox, simulationContentLayout.rowCount() - 1, 2);
    // number of steps
    simulationContentLayout.addWidget(new MyLabel("Time Points:"), simulationContentLayout.rowCount(), 0);
    timePointsSpinBox.setSpecialValueText("");
    timePointsSpinBox.setRange(1, 100000);
    timePointsSpinBox.setFixedWidth(75.0);
    simulationContentLayout.addWidget(&timePointsSpinBox, simulationContentLayout.rowCount() - 1, 2);
    
    // reset button
    resetButton.setText("Reset");
    simulateButton.setFixedWidth(85.0);
    connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetInfo()));
    connect(&resetButton, SIGNAL(clicked()), _mw, SLOT(resetSimulation()));
    simulationContentLayout.addWidget(&resetButton, simulationContentLayout.rowCount(), 0);
    
    // pause/ resume button
    pauseOrResumeButton.setFixedWidth(85.0);
    connect(&pauseOrResumeButton, SIGNAL(paused()), _mw, SLOT(pauseGraphicalSpeciesFillColorAnimation()));
    connect(&pauseOrResumeButton, SIGNAL(resumed()), _mw, SLOT(resumeGraphicalSpeciesFillColorAnimation()));
    simulationContentLayout.addWidget(&pauseOrResumeButton, simulationContentLayout.rowCount() - 1, 1);
    
    // simulate button
    simulateButton.setText("Simulate");
    simulateButton.setFixedWidth(85.0);
    connect(&simulateButton, SIGNAL(clicked()), _mw, SLOT(resetSimulation()));
    connect(&simulateButton, SIGNAL(clicked()), this, SLOT(simulate()));
    simulationContentLayout.addWidget(&simulateButton, simulationContentLayout.rowCount() - 1, 2);
    
    // simulation time slider
    simulationTimeSlider.setOrientation(Qt::Horizontal);
    connect(_mw, SIGNAL(currentTimeChanged(int)), &simulationTimeSlider, SLOT(setValue(int)));
    connect(&simulationTimeSlider, SIGNAL(sliderPressed()), &pauseOrResumeButton, SLOT(pause()));
    connect(&simulationTimeSlider, SIGNAL(sliderReleased()), &pauseOrResumeButton, SLOT(resume()));
    connect(&simulationTimeSlider, SIGNAL(sliderMoved(int)), _mw, SLOT(setCurrentTime(int)));
    if (_mw && _mw->isSetGSpeciesFillColorAnimation()) {
        connect(_mw->getGSpeciesFillColorAnimation(), SIGNAL(finished()), &pauseOrResumeButton, SLOT(pause()));
    }
    simulationContentLayout.addWidget(&simulationTimeSlider, simulationContentLayout.rowCount(), 0, 1, 3);
    
    simulationContentLayout.setAlignment(Qt::AlignTop);
    simulationBranch->setLayout(&simulationContentLayout);
    
    /// features
    featuresBranch = new MyGroupBox(tabMenu);
    // id
    featuresContentLayout.addWidget(new MyLabel("Id:"), featuresContentLayout.rowCount() - 1, 0);
    featuresContentLayout.setAlignment(Qt::AlignTop);
    featuresBranch->setLayout(&featuresContentLayout);
    
    // tabs
    tabMenu->addTab(simulationBranch, "Simulation");
    tabMenu->addTab(featuresBranch, "Features");
    qLayout.addItem(new QSpacerItem(0, 10), qLayout.rowCount(), 0);
    qLayout.addWidget(tabMenu, qLayout.rowCount(), 0);
}

void ModelFeatureMenu::showInfo() {
    resetInfo();
    show();
}

void ModelFeatureMenu::resetInfo() {
    startTimeSpinBox.setValue(startTimeSpinBox.minimum());
    endTimeSpinBox.setValue(5.0);
    timePointsSpinBox.setValue(10.0);
    pauseOrResumeButton.disable();
    simulationTimeSlider.setRange(0, 1);
    simulationTimeSlider.setSliderPosition(0);
    simulationTimeSlider.setEnabled(false);
}

void ModelFeatureMenu::simulate() {
    if (_mw && _mw->getSBMLDocument() && !(_mw->getSBMLDocument()->simulateTelluriumSBMLDocument(_mw, startTimeSpinBox.value(), endTimeSpinBox.value(), timePointsSpinBox.value()))) {
        pauseOrResumeButton.enable();
        simulationTimeSlider.setRange(startTimeSpinBox.value(), endTimeSpinBox.value());
        simulationTimeSlider.setEnabled(true);
        _mw->animateSpeciesConcentrations();
    }
}

// Item Feature Menu

ItemFeatureMenuElement::ItemFeatureMenuElement(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    _gObject = NULL;
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gText = NULL;
    modelBranch = NULL;
    layoutBranch = NULL;
    renderStyleBranch = NULL;
    renderGeometricShapeBranch = NULL;
    renderTreeView = NULL;
    textBranch = NULL;
    textTreeView = NULL;
    bBoxFMenu = NULL;
    curveFMenu = NULL;
    strokeFMenu = NULL;
    fillFMenu = NULL;
    _gSFMenuElements.clear();
    _textFMenus.clear();
    setFixedSize(QSize(450, maximumHeight()));
    
    tabMenu = new MyTabWidget(this);
    modelIdLineEdit.setReadOnly(true);
    
    // layout features
    connect(&layoutIdLineEdit, SIGNAL(editingFinished()), this, SLOT(changeLayoutId()));
    
    // bounding box features
    connect(&boundingBoxAddOrRemoveButton, SIGNAL(clicked()), this, SLOT(addOrRemoveBoundingBox()));
    
    // curve features
    connect(&curveAddOrRemoveButton, SIGNAL(clicked()), this, SLOT(addOrRemoveCurve()));
    
    // style features
    connect(&styleAddOrRemoveButton, SIGNAL(clicked()), this, SLOT(addOrRemoveStyle()));
}

void ItemFeatureMenuElement::hideInfo() {
    FeatureMenuElement::hideInfo();
    if (renderTreeView)
        renderTreeView->collapseAll();
    
    if (bBoxFMenu)
        bBoxFMenu->collapseTree();
    
    if (curveFMenu)
        curveFMenu->collapseTree();
    
    for (constGSFMenuElementIt gSIt = gSFMenuElementsBegin(); gSIt != gSFMenuElementsEnd(); ++gSIt)
        (*gSIt)->collapseTree();
}

void ItemFeatureMenuElement::updateStyle(VLocalStyle* style) {
    if (_gObject && style) {
        _gObject->setStyle(style);
        updateValues();
    }
}

void ItemFeatureMenuElement::setStyle() {
    if (_gObject) {
        _gObject->setStyle(_mw, true);
        updateValues();
    }
}

void ItemFeatureMenuElement::unSetStyle() {
    if (_gObject) {
        _gObject->unSetStyle();
        updateValues();
    }
}

void ItemFeatureMenuElement::clearGSFMenuElements() {
    for (constGSFMenuElementIt gSFMIt = gSFMenuElementsBegin(); gSFMIt != gSFMenuElementsEnd(); ++ gSFMIt) {
        (*gSFMIt)->resetValues();
        (*gSFMIt)->deleteLater();
    }
    _gSFMenuElements.clear();
}

void ItemFeatureMenuElement::clearTextFMenus() {
    for (constTextFMenuIt tFMIt = textFMenuBegin(); tFMIt != textFMenuEnd(); ++ tFMIt) {
        (*tFMIt)->resetValues();
        (*tFMIt)->deleteLater();
    }
    _textFMenus.clear();
}

void ItemFeatureMenuElement::changeLayoutId() {
    if (_mw && _mw->isSetSBMLDocument() && !stringCompare(layoutIdLineEdit.text().toStdString(), ne_go_getGlyphId(_gO)) && !ne_go_setGlyphId(_mw->getSBMLDocument()->getNetwork(), _gO, layoutIdLineEdit.text().toStdString())) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        updateValues();
    }
}

void ItemFeatureMenuElement::addOrRemoveBoundingBox() {
    if (boundingBoxAddOrRemoveButton.text() == "+")
        addBoundingBox();
    else if (boundingBoxAddOrRemoveButton.text() == "-")
        removeBoundingBox();
}

void ItemFeatureMenuElement::addBoundingBox(const double& x, const double& y, const double& width, const double& height) {
    if (_gO && bBoxFMenu) {
        if (_mw && _mw->isSetSBMLDocument()) {
            LBox* bBox = NULL;
            if (ne_go_getBoundingBox(_gO))
                bBox = ne_go_getBoundingBox(_gO);
            
            if (ne_go_getType(_gO) == 2) {
                bBox = new LBox(x, y, width, height);
                ne_rxn_unSetCurve((NReaction*)(_gO), false);
            }
            
            if (bBox) {
                ne_go_setBoundingBox(_gO, bBox);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
    }
}

void ItemFeatureMenuElement::removeBoundingBox() {
    if (_gO && bBoxFMenu) {
        if (_gText && ne_go_getNumTexts(_gO)) {
            NText* text = ne_go_getText(_gO, 0);
            if (_gText->isSetPlainText())
                ne_gtxt_setPlainText(text, _gText->getPlainText());
            _gText->setNGraphicalObject(text);
        }
        
        bBoxFMenu->collapseTree();
        ne_go_unSetBoundingBox(_gO, false);
        _mw->getSBMLDocument()->setLayoutModified(true);
        updateValues();
    }
}

void ItemFeatureMenuElement::addOrRemoveCurve() {
    if (curveFMenu) {
        if (curveAddOrRemoveButton.text() == "-") {
            if (_gO->getType() == 2 && ne_rxn_isSetCurve((NReaction*)_gO)) {
                ne_rxn_unSetCurve((NReaction*)_gO, false);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
            else if (_gO->getType() == 3 && ne_sr_isSetCurve((NSpeciesReference*)_gO)) {
                ne_sr_unSetCurve((NSpeciesReference*)_gO, false);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
        else if (curveAddOrRemoveButton.text() == "+") {
            LCurve* curve = NULL;
            if (_gO->getType() == 2 && ne_rxn_getCurve((NReaction*)_gO))
                curve = ne_rxn_getCurve((NReaction*)_gO);
            else if (_gO->getType() == 3 && ne_sr_getCurve((NSpeciesReference*)_gO))
                curve = ne_sr_getCurve((NSpeciesReference*)_gO);
            
            if (!curve) {
                curve = new LCurve();
                ne_crv_addElement(curve, 0, new LPoint(), new LPoint());
            }
            
            if (_gO->getType() == 2 && !ne_rxn_setCurve((NReaction*)_gO, curve)) {
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
            else if (_gO->getType() == 3 && !ne_sr_setCurve((NSpeciesReference*)_gO, curve)) {
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
    }
}

void ItemFeatureMenuElement::addOrRemoveStyle() {
    if (_gO) {
        if (styleAddOrRemoveButton.text() == "-") {
            if (renderTreeView)
                renderTreeView->collapseAll();
            unSetStyle();
        }
        else if (styleAddOrRemoveButton.text() == "+")
            setStyle();
    }
}

void ItemFeatureMenuElement::addGeometricShape(const QString& geometricShape) {
    if (_mw && _mw->isSetSBMLDocument()) {
        QString imageFileName;
        int numberOfPolygonVertices = -1;
        
        if (geometricShape == "Image")
            imageFileName = QFileDialog::getOpenFileName(this, "Select an image file", ".", tr("Image Files (*.png *.jpg *.jpeg)"));
        else if (geometricShape == "Triangle")
            numberOfPolygonVertices = 3;
        else if (geometricShape == "Diamond")
            numberOfPolygonVertices = 4;
        else if (geometricShape == "Pentagon")
            numberOfPolygonVertices = 5;
        else if (geometricShape == "Hexagon")
            numberOfPolygonVertices = 6;
        else if (geometricShape == "Octagon")
            numberOfPolygonVertices = 8;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (style) {
                if (geometricShape == "Rectangle")
                    ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_REC);
                else if (geometricShape == "Ellipse")
                    ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_ELP);
                else if (geometricShape == "RenderCurve")
                    ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_CRV);
                else if (!imageFileName.isEmpty())
                    ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_IMG, false, -1, imageFileName.toStdString());
                else if (numberOfPolygonVertices != -1)
                    ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_PLG, false, numberOfPolygonVertices);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                updateStyle(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (lineEnding) {
                if (geometricShape == "Rectangle")
                    ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_REC, true);
                else if (geometricShape == "Ellipse")
                    ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_ELP, true);
                else if (geometricShape == "RenderCurve")
                    ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_CRV);
                else if (!imageFileName.isEmpty())
                    ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_IMG, true, -1, imageFileName.toStdString());
                else if (numberOfPolygonVertices != -1)
                    ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_PLG, true, numberOfPolygonVertices);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                updateLineEnding(lineEnding);
            }
        }
    }
}

void ItemFeatureMenuElement::removeGeometricShape(const unsigned int& itemIndex) {
    if (_mw && _mw->isSetSBMLDocument()) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_grp_removeGeometricShape(ne_stl_getGroup(style), itemIndex)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                updateStyle(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_grp_removeGeometricShape(ne_le_getGroup(lineEnding), itemIndex)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                updateLineEnding(lineEnding);
            }
        }
    }
}

void ItemFeatureMenuElement::addText(const QString& plainText) {
    if (_mw && _mw->isSetSBMLDocument()) {
        if (ne_go_addText(_mw->getSBMLDocument()->getNetwork(), _gO, NULL, plainText.toStdString())) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            updateValues();
        }
    }
}

void ItemFeatureMenuElement::removeText(const unsigned int& itemIndex) {
    if (_mw && _mw->isSetSBMLDocument() && _gO) {
        if (!ne_go_removeText(_mw->getSBMLDocument()->getNetwork(), _gO, itemIndex)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            updateValues();
        }
    }
}

// Species Feature Menu

SpeciesFeatureMenu::SpeciesFeatureMenu(QWidget* parent, MainWindow* mw) : ItemFeatureMenuElement(parent, mw) {
    titleLabel.setText("Species");
    
    /// model features
    modelBranch = new MyGroupBox(tabMenu);
    // id
    modelContentLayout.addWidget(new MyLabel("Id:"), modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    // comparment
    modelContentLayout.addWidget(new MyLabel("Compartment:"), modelContentLayout.rowCount(), 0);
    compartmentIdLineEdit.setReadOnly(true);
    modelContentLayout.addWidget(&compartmentIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    modelContentLayout.setAlignment(Qt::AlignTop);
    modelBranch->setLayout(&modelContentLayout);
    
    /// layout features
    layoutBranch = new MyGroupBox(tabMenu);
    // id
    layoutContentLayout.addWidget(new MyLabel("Layout Id:"), layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdLineEdit, layoutContentLayout.rowCount() - 1, 1);
    layoutContentLayout.addItem(new QSpacerItem(0, 20), layoutContentLayout.rowCount(), 0);
    // bounding box
    layoutContentLayout.addWidget(new MyLabel("Bounding Box"), layoutContentLayout.rowCount(), 0);
    layoutContentLayout.addWidget(&boundingBoxAddOrRemoveButton, layoutContentLayout.rowCount() - 1, 1, Qt::AlignRight);
    bBoxFMenu = new BoundingBoxFeatureMenu(layoutBranch, _mw);
    connect(bBoxFMenu, SIGNAL(bBoxPositionChanged()), SLOT(updateValues()));
    connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged()), SLOT(updateValues()));
    //connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged()), SLOT(clearGeometricShapes()));
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 2);
    layoutContentLayout.setAlignment(Qt::AlignTop);
    layoutBranch->setLayout(&layoutContentLayout);
    
    /// render features
    renderStyleBranch = new MyGroupBox(tabMenu);
    renderTreeView = new MyTreeView(renderStyleBranch);
    // style
    renderStyleContentLayout.addWidget(new MyLabel("Style"), renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderStyleContentLayout.addWidget(&styleAddOrRemoveButton, renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    // stroke
    strokeFMenu = new StrokeFeatureMenu(renderTreeView, _mw);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(strokeFMenu, "Stroke");
    // fill
    fillFMenu = new FillFeatureMenu(renderTreeView, _mw);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(fillFMenu, "Fill");
    // geometric shape
    renderGeometricShapeBranch = new MyGroupBox(renderTreeView);
    renderGeometricShapeContentLayout.addWidget(&addRemoveGeometricShapeButtons, renderGeometricShapeContentLayout.rowCount(), 2);
    connect(&addRemoveGeometricShapeButtons, SIGNAL(geometricShapeChosen(const QString&)), this, SLOT(addGeometricShape(const QString&)));
    connect(&addRemoveGeometricShapeButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeGeometricShape(const unsigned int&)));
    renderGeometricShapeBranch->setLayout(&renderGeometricShapeContentLayout);
    renderTreeView->addBranchWidget(renderGeometricShapeBranch, "Geometric Shapes");
    renderStyleContentLayout.addWidget(renderTreeView, renderStyleContentLayout.rowCount(), 0, 1, 2);
    renderStyleBranch->setLayout(&renderStyleContentLayout);
    
    /// text features
    textBranch = new MyGroupBox(tabMenu);
    textTreeView = new MyTreeView(tabMenu);
    // text
    textContentLayout.addWidget(new MyLabel("Texts"), textContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    textContentLayout.addWidget(&addRemoveTextButtons, textContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    connect(&addRemoveTextButtons, SIGNAL(textChosen(const QString&)), this, SLOT(addText(const QString&)));
    connect(&addRemoveTextButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeText(const unsigned int&)));
    textContentLayout.addWidget(textTreeView, textContentLayout.rowCount(), 0, 1, 2);
    textBranch->setLayout(&textContentLayout);
    
    /// tab menu
    tabMenu->addTab(modelBranch, "Model");
    tabMenu->addTab(layoutBranch, "Layout");
    tabMenu->addTab(renderStyleBranch, "Render");
    tabMenu->addTab(textBranch, "Text");
    qLayout.addItem(new QSpacerItem(0, 10), qLayout.rowCount(), 0);
    qLayout.addWidget(tabMenu, qLayout.rowCount(), 0);
    
    // reset values;
    resetValues();
}

void SpeciesFeatureMenu::showInfo(GraphicalObjectBase* gObject) {
    // reset values
    resetValues();
    
    if (gObject) {
        _gObject = gObject;

        if (gObject->isSetNGraphicalObject()) {
            _gO = _gObject->getNGraphicalObject();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLineEdit.setText(ne_ne_getId(_gO).c_str());
            // set compartment
            if (ne_spc_isSetCompartment((NSpecies*)_gO))
                compartmentIdLineEdit.setText(ne_spc_getCompartment((NSpecies*)_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdLineEdit.setText(ne_go_getGlyphId(_gO).c_str());
            // bounding box
            if (ne_go_isSetBoundingBox(_gO)) {
                bBoxFMenu->showInfo(_gO);
                boundingBoxAddOrRemoveButton.enableRemove();
            }
            
            /// render features
            // species
            if (_gObject->isSetStyle()) {
                _style = _gObject->getStyle();
                styleAddOrRemoveButton.enableRemove();
                if (renderTreeView)
                    renderTreeView->showColumn(0);
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                if (ne_go_isSetBoundingBox(_gO)) {
                    // fill features
                    fillFMenu->showInfo(_gO, _style);
                    // geometric shape features
                    if (ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style)))
                        addRemoveGeometricShapeButtons.enableRemoveButton(ne_stl_getGroup(_style));
                    GeometricShapeFeatureMenuElement* gSFMenuElement = NULL;
                    for (int i = 0; i < ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style)); ++i) {
                        gSFMenuElement = NULL;
                        // set geometric shape feature menu
                        switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(_style), i))) {
                            case 0:
                                gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Image", "Geometric Shapes");
                                ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 1:
                                gSFMenuElement = new RenderCurveGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": RenderCurve", "Geometric Shapes");
                                ((RenderCurveGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            //case 2:
                                //gSFMenuElement = new TextGeometricShapeFeatureMenu(this, _mw);
                                //break;
                                
                            case 3:
                                gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Rectangle", "Geometric Shapes");
                                ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 4:
                                gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Ellipse", "Geometric Shapes");
                                ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 5:
                                gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Polygon", "Geometric Shapes");
                                ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (gSFMenuElement) {
                            _gSFMenuElements.push_back(gSFMenuElement);
                            connect(gSFMenuElement, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));}
                    }
                }
            }
            
            // text features
            TextFeatureMenu* textFMenu = NULL;
            if (_gObject->getNumGraphicalTexts())
                addRemoveTextButtons.enableRemoveButton(_gObject->getNumGraphicalTexts());
            if (ne_go_isSetBoundingBox(_gO))
                addRemoveTextButtons.enableAddButton(_gO);
            for (GraphicalObjectBase::constGTextIt gTIt = _gObject->gTextsBegin(); gTIt != _gObject->gTextsEnd(); ++gTIt) {
                textFMenu = new TextFeatureMenu(this, _mw);
                textTreeView->addBranchWidget(textFMenu, "Text " + QString::number(gTIt - gObject->gTextsBegin() + 1) + ":");
                textFMenu->showInfo(*gTIt, _gO);
                _textFMenus.push_back(textFMenu);
                connect(textFMenu, SIGNAL(textChanged()), SLOT(updateValues()));
            }
        }
    }
    
    show();
}

void SpeciesFeatureMenu::resetValues() {
    modelIdLineEdit.setText("N/A");
    compartmentIdLineEdit.setText("N/A");
    layoutIdLineEdit.clear();
    boundingBoxAddOrRemoveButton.resetValues();
    if (bBoxFMenu)
        bBoxFMenu->resetValues();
    styleAddOrRemoveButton.resetValues();
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    clearGSFMenuElements();
    if (renderTreeView) {
        renderTreeView->removeBranches("Geometric Shapes", 1);
        renderTreeView->hideColumn(0);
    }
    addRemoveGeometricShapeButtons.resetValues();
    clearTextFMenus();
    if (textTreeView)
        textTreeView->removeBranches();
    addRemoveTextButtons.resetValues();
    
    _gObject = NULL;
    _gO = NULL;
    _gText = NULL;
    _style = NULL;
}

void SpeciesFeatureMenu::updateValues() {
    if (_gObject) {
        ((GraphicalSpecies*)_gObject)->updateValues(_mw, true);
        showInfo(_gObject);
    }
}

// Compartment Feature Menu

CompartmentFeatureMenu::CompartmentFeatureMenu(QWidget* parent, MainWindow* mw) : ItemFeatureMenuElement(parent, mw) {
    titleLabel.setText("Compartment");
    
    /// model features
    modelBranch = new MyGroupBox(tabMenu);
    // id
    modelContentLayout.addWidget(new MyLabel("Id:"), modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    modelContentLayout.setAlignment(Qt::AlignTop);
    modelBranch->setLayout(&modelContentLayout);
    
    /// layout features
    layoutBranch = new MyGroupBox(tabMenu);
    // id
    layoutContentLayout.addWidget(new MyLabel("Layout Id:"), layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdLineEdit, layoutContentLayout.rowCount() - 1, 1);
    layoutContentLayout.addItem(new QSpacerItem(0, 20), layoutContentLayout.rowCount(), 0);
    // bounding box
    layoutContentLayout.addWidget(new MyLabel("Bounding Box"), layoutContentLayout.rowCount(), 0);
    layoutContentLayout.addWidget(&boundingBoxAddOrRemoveButton, layoutContentLayout.rowCount() - 1, 1, Qt::AlignRight);
    bBoxFMenu = new BoundingBoxFeatureMenu(layoutBranch, _mw);
    connect(bBoxFMenu, SIGNAL(bBoxPositionChanged()), SLOT(updateValues()));
    connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged()), SLOT(updateValues()));
    //connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged()), SLOT(clearGeometricShapes()));
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 2);
    layoutContentLayout.setAlignment(Qt::AlignTop);
    layoutBranch->setLayout(&layoutContentLayout);
    
    /// render features
    renderStyleBranch = new MyGroupBox(tabMenu);
    renderTreeView = new MyTreeView(renderStyleBranch);
    // style
    renderStyleContentLayout.addWidget(new MyLabel("Style"), renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderStyleContentLayout.addWidget(&styleAddOrRemoveButton, renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    // stroke
    strokeFMenu = new StrokeFeatureMenu(renderTreeView, _mw);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(strokeFMenu, "Stroke");
    // fill
    fillFMenu = new FillFeatureMenu(renderTreeView, _mw);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(fillFMenu, "Fill");
    // geometric shape
    renderGeometricShapeBranch = new MyGroupBox(renderTreeView);
    renderGeometricShapeContentLayout.addWidget(&addRemoveGeometricShapeButtons, renderGeometricShapeContentLayout.rowCount(), 2);
    connect(&addRemoveGeometricShapeButtons, SIGNAL(geometricShapeChosen(const QString&)), this, SLOT(addGeometricShape(const QString&)));
    connect(&addRemoveGeometricShapeButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeGeometricShape(const unsigned int&)));
    renderGeometricShapeBranch->setLayout(&renderGeometricShapeContentLayout);
    renderTreeView->addBranchWidget(renderGeometricShapeBranch, "Geometric Shapes");
    renderStyleContentLayout.addWidget(renderTreeView, renderStyleContentLayout.rowCount(), 0, 1, 2);
    renderStyleBranch->setLayout(&renderStyleContentLayout);
    
    /// text features
    textBranch = new MyGroupBox(tabMenu);
    textTreeView = new MyTreeView(tabMenu);
    // text
    textContentLayout.addWidget(new MyLabel("Texts"), textContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    textContentLayout.addWidget(&addRemoveTextButtons, textContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    connect(&addRemoveTextButtons, SIGNAL(textChosen(const QString&)), this, SLOT(addText(const QString&)));
    connect(&addRemoveTextButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeText(const unsigned int&)));
    textContentLayout.addWidget(textTreeView, textContentLayout.rowCount(), 0, 1, 2);
    textBranch->setLayout(&textContentLayout);
    
    /// tab menu
    tabMenu->addTab(modelBranch, "Model");
    tabMenu->addTab(layoutBranch, "Layout");
    tabMenu->addTab(renderStyleBranch, "Render");
    tabMenu->addTab(textBranch, "Text");
    qLayout.addItem(new QSpacerItem(0, 10), qLayout.rowCount(), 0);
    qLayout.addWidget(tabMenu, qLayout.rowCount(), 0);
    
    // reset values;
    resetValues();
}

void CompartmentFeatureMenu::showInfo(GraphicalObjectBase* gObject) {
    // reset values
    resetValues();
    
    if (gObject) {
        _gObject = gObject;
        
        if (_gObject->isSetNGraphicalObject()) {
            _gO = _gObject->getNGraphicalObject();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLineEdit.setText(ne_ne_getId(_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdLineEdit.setText(ne_go_getGlyphId(_gO).c_str());
            // bounding box
            if (ne_go_isSetBoundingBox(_gO)) {
                bBoxFMenu->showInfo(_gO);
                boundingBoxAddOrRemoveButton.enableRemove();
            }
            
            /// render features
            // compartment
            if (_gObject->isSetStyle()) {
                _style = _gObject->getStyle();
                styleAddOrRemoveButton.enableRemove();
                if (renderTreeView)
                    renderTreeView->showColumn(0);
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                if (ne_go_isSetBoundingBox(_gO)) {
                    // fill features
                    fillFMenu->showInfo(_gO, _style);
                    // geometric shape features
                    if (ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style)))
                        addRemoveGeometricShapeButtons.enableRemoveButton(ne_stl_getGroup(_style));
                    GeometricShapeFeatureMenuElement* gSFMenuElement = NULL;
                    for (int i = 0; i < ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style)); ++i) {
                        gSFMenuElement = NULL;
                        // set geometric shape feature menu
                        switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(_style), i))) {
                            case 0:
                                gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Image", "Geometric Shapes");
                                ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 1:
                                gSFMenuElement = new RenderCurveGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": RenderCurve", "Geometric Shapes");
                                ((RenderCurveGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            //case 2:
                                //gSFMenuElement = new TextGeometricShapeFeatureMenu(this, _mw);
                                //break;
                                
                            case 3:
                                gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Rectangle", "Geometric Shapes");
                                ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 4:
                                gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Ellipse", "Geometric Shapes");
                                ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 5:
                                gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Polygon", "Geometric Shapes");
                                ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (gSFMenuElement) {
                            _gSFMenuElements.push_back(gSFMenuElement);
                            connect(gSFMenuElement, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));}
                    }
                }
            }
            
            // text features
            TextFeatureMenu* textFMenu = NULL;
            if (_gObject->getNumGraphicalTexts())
                addRemoveTextButtons.enableRemoveButton(_gObject->getNumGraphicalTexts());
            if (ne_go_isSetBoundingBox(_gO))
                addRemoveTextButtons.enableAddButton(_gO);
            for (GraphicalObjectBase::constGTextIt gTIt = _gObject->gTextsBegin(); gTIt != _gObject->gTextsEnd(); ++gTIt) {
                textFMenu = new TextFeatureMenu(this, _mw);
                textTreeView->addBranchWidget(textFMenu, "Text " + QString::number(gTIt - gObject->gTextsBegin() + 1) + ":"  , "Texts");
                textFMenu->showInfo(*gTIt, _gO);
                _textFMenus.push_back(textFMenu);
                connect(textFMenu, SIGNAL(textChanged()), SLOT(updateValues()));
            }
        }
    }
    
    show();
}

void CompartmentFeatureMenu::resetValues() {
    modelIdLineEdit.setText("N/A");
    layoutIdLineEdit.clear();
    boundingBoxAddOrRemoveButton.resetValues();
    if (bBoxFMenu)
        bBoxFMenu->resetValues();
    styleAddOrRemoveButton.resetValues();
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    clearGSFMenuElements();
    if (renderTreeView) {
        renderTreeView->removeBranches("Geometric Shapes", 1);
        renderTreeView->hideColumn(0);
    }
    addRemoveGeometricShapeButtons.resetValues();
    clearTextFMenus();
    if (textTreeView)
        textTreeView->removeBranches();
    addRemoveTextButtons.resetValues();
    
    _gObject = NULL;
    _gO = NULL;
    _gText = NULL;
    _style = NULL;
}

void CompartmentFeatureMenu::updateValues() {
    if (_gObject) {
        ((GraphicalCompartment*)_gObject)->updateValues(_mw);
        showInfo(_gObject);
    }
}

// Reaction Feature Menu

ReactionFeatureMenu::ReactionFeatureMenu(QWidget* parent, MainWindow* mw) : ItemFeatureMenuElement(parent, mw) {
    
    titleLabel.setText("Reaction");
    tabMenu = new MyTabWidget(this);
    
    /// model features
    modelBranch = new MyGroupBox(tabMenu);
    // id
    modelContentLayout.addWidget(new MyLabel("Id:"), modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    modelContentLayout.setAlignment(Qt::AlignTop);
    modelBranch->setLayout(&modelContentLayout);
    /// layout features
    layoutBranch = new MyGroupBox(tabMenu);
    // id
    layoutContentLayout.addWidget(new MyLabel("Layout Id:"), layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdLineEdit, layoutContentLayout.rowCount() - 1, 1);
    layoutContentLayout.addItem(new QSpacerItem(0, 20), layoutContentLayout.rowCount(), 0);
    // curve
    layoutContentLayout.addWidget(new MyLabel("Curve"), layoutContentLayout.rowCount(), 0);
    layoutContentLayout.addWidget(&curveAddOrRemoveButton, layoutContentLayout.rowCount() - 1, 1, Qt::AlignRight);
    curveFMenu = new CurveFeatureMenu(layoutBranch, _mw);
    connect(curveFMenu, SIGNAL(curveChanged()), SLOT(updateValues()));
    layoutContentLayout.addWidget(curveFMenu, layoutContentLayout.rowCount(), 0, 1, 2);
    // bounding box
    layoutContentLayout.addWidget(new MyLabel("Bounding Box"), layoutContentLayout.rowCount(), 0);
    layoutContentLayout.addWidget(&addOrRemoveReactionBoundingBoxButton, layoutContentLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&addOrRemoveReactionBoundingBoxButton, SIGNAL(boundingBoxFeaturesChosen(const double&, const double&, const double&, const double& )), this, SLOT(addBoundingBox(const double&, const double&, const double&, const double&)));
    connect(&addOrRemoveReactionBoundingBoxButton, SIGNAL(removeBoundingBox()), this, SLOT(removeBoundingBox()));
    bBoxFMenu = new BoundingBoxFeatureMenu(layoutBranch, _mw);
    connect(bBoxFMenu, SIGNAL(bBoxPositionChanged()), SLOT(updateValues()));
    connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged()), SLOT(updateValues()));
    //connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged()), SLOT(clearGeometricShapes()));
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 2);
    layoutContentLayout.setAlignment(Qt::AlignTop);
    layoutBranch->setLayout(&layoutContentLayout);
    
    /// render features
    renderStyleBranch = new MyGroupBox(tabMenu);
    renderTreeView = new MyTreeView(renderStyleBranch);
    // style
    renderStyleContentLayout.addWidget(new MyLabel("Style"), renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderStyleContentLayout.addWidget(&styleAddOrRemoveButton, renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    // stroke
    strokeFMenu = new StrokeFeatureMenu(renderTreeView, _mw);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(strokeFMenu, "Stroke");
    // fill
    fillFMenu = new FillFeatureMenu(renderTreeView, _mw);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(fillFMenu, "Fill");
    // geometric shape
    renderGeometricShapeBranch = new MyGroupBox(renderTreeView);
    renderGeometricShapeContentLayout.addWidget(&addRemoveGeometricShapeButtons, renderGeometricShapeContentLayout.rowCount(), 2);
    connect(&addRemoveGeometricShapeButtons, SIGNAL(geometricShapeChosen(const QString&)), this, SLOT(addGeometricShape(const QString&)));
    connect(&addRemoveGeometricShapeButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeGeometricShape(const unsigned int&)));
    renderGeometricShapeBranch->setLayout(&renderGeometricShapeContentLayout);
    renderTreeView->addBranchWidget(renderGeometricShapeBranch, "Geometric Shapes");
    renderStyleContentLayout.addWidget(renderTreeView, renderStyleContentLayout.rowCount(), 0, 1, 2);
    renderStyleBranch->setLayout(&renderStyleContentLayout);
    
    /// text features
    textBranch = new MyGroupBox(tabMenu);
    textTreeView = new MyTreeView(tabMenu);
    // text
    textContentLayout.addWidget(new MyLabel("Texts"), textContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    textContentLayout.addWidget(&addRemoveTextButtons, textContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    connect(&addRemoveTextButtons, SIGNAL(textChosen(const QString&)), this, SLOT(addText(const QString&)));
    connect(&addRemoveTextButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeText(const unsigned int&)));
    textContentLayout.addWidget(textTreeView, textContentLayout.rowCount(), 0, 1, 2);
    textBranch->setLayout(&textContentLayout);
    
    /// tab menu
    tabMenu->addTab(modelBranch, "Model");
    tabMenu->addTab(layoutBranch, "Layout");
    tabMenu->addTab(renderStyleBranch, "Render");
    tabMenu->addTab(textBranch, "Text");
    qLayout.addItem(new QSpacerItem(0, 10), qLayout.rowCount(), 0);
    qLayout.addWidget(tabMenu, qLayout.rowCount(), 0);
    
    // reset values;
    resetValues();
}

void ReactionFeatureMenu::showInfo(GraphicalObjectBase* gObject) {
    // reset values
    resetValues();
    
    if (gObject) {
        _gObject = gObject;
        
        if (_gObject->isSetNGraphicalObject()) {
            _gO = _gObject->getNGraphicalObject();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLineEdit.setText(ne_ne_getId(_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdLineEdit.setText(ne_go_getGlyphId(_gO).c_str());
            // set curve
            addOrRemoveReactionBoundingBoxButton.setBoxRanges(_mw);
            if (ne_rxn_isSetCurve((NReaction*)_gO)) {
                curveFMenu->showInfo(ne_rxn_getCurve((NReaction*)_gO));
                curveAddOrRemoveButton.enableRemove();
                addOrRemoveReactionBoundingBoxButton.enableFromCurveExtents(ne_crv_getExtentBox(ne_rxn_getCurve((NReaction*)_gO)));
            }
            // set bounding box
            else if (ne_go_isSetBoundingBox(_gO)) {
                bBoxFMenu->showInfo(_gO);
                addOrRemoveReactionBoundingBoxButton.enableRemove();
            }
            if (ne_go_getBoundingBox(_gO))
                addOrRemoveReactionBoundingBoxButton.initialize(ne_go_getBoundingBox(_gO));
            
            /// render features
            // reaction
            if (_gObject->isSetStyle()) {
                _style = _gObject->getStyle();
                styleAddOrRemoveButton.enableRemove();
                if (renderTreeView)
                    renderTreeView->showColumn(0);
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                if (ne_go_isSetBoundingBox(_gO)) {
                    // fill features
                    fillFMenu->showInfo(_gO, _style);
                    // geometric shape features
                    addRemoveGeometricShapeButtons.enableAddButton();
                    if (ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style)))
                        addRemoveGeometricShapeButtons.enableRemoveButton(ne_stl_getGroup(_style));
                    GeometricShapeFeatureMenuElement* gSFMenuElement = NULL;
                    for (int i = 0; i < ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style)); ++i) {
                        gSFMenuElement = NULL;
                        // set geometric shape feature menu
                        switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(_style), i))) {
                            case 0:
                                gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Image", "Geometric Shapes");
                                ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 1:
                                gSFMenuElement = new RenderCurveGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": RenderCurve", "Geometric Shapes");
                                ((RenderCurveGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            //case 2:
                                //gSFMenuElement = new TextGeometricShapeFeatureMenu(this, _mw);
                                //break;
                                
                            case 3:
                                gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Rectangle", "Geometric Shapes");
                                ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 4:
                                gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Ellipse", "Geometric Shapes");
                                ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            case 5:
                                gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                                renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Polygon", "Geometric Shapes");
                                ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style, i);
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (gSFMenuElement) {
                            _gSFMenuElements.push_back(gSFMenuElement);
                            connect(gSFMenuElement, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
                        }
                    }
                }
            }
            
            // text features
            TextFeatureMenu* textFMenu = NULL;
            if (_gObject->getNumGraphicalTexts())
                addRemoveTextButtons.enableRemoveButton(_gObject->getNumGraphicalTexts());
            if (ne_go_isSetBoundingBox(_gO))
                addRemoveTextButtons.enableAddButton(_gO);
            for (GraphicalObjectBase::constGTextIt gTIt = _gObject->gTextsBegin(); gTIt != _gObject->gTextsEnd(); ++gTIt) {
                textFMenu = new TextFeatureMenu(this, _mw);
                textTreeView->addBranchWidget(textFMenu, "Text " + QString::number(gTIt - gObject->gTextsBegin() + 1) + ":"  , "Texts");
                textFMenu->showInfo(*gTIt, _gO);
                _textFMenus.push_back(textFMenu);
                connect(textFMenu, SIGNAL(textChanged()), SLOT(updateValues()));
            }
        }
    }
    
    show();
}

void ReactionFeatureMenu::resetValues() {
    modelIdLineEdit.setText("N/A");
    layoutIdLineEdit.clear();
    curveAddOrRemoveButton.resetValues();
    if (curveFMenu)
        curveFMenu->resetValues();
    addOrRemoveReactionBoundingBoxButton.resetValues();
    if (bBoxFMenu)
        bBoxFMenu->resetValues();
    styleAddOrRemoveButton.resetValues();
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    clearGSFMenuElements();
    if (renderTreeView) {
        renderTreeView->removeBranches("Geometric Shapes", 1);
        renderTreeView->hideColumn(0);
    }
    addRemoveGeometricShapeButtons.resetValues(true);
    clearTextFMenus();
    if (textTreeView)
        textTreeView->removeBranches();
    addRemoveTextButtons.resetValues();
    
    _gObject = NULL;
    _gO = NULL;
    _gText = NULL;
    _style = NULL;
}

void ReactionFeatureMenu::updateValues() {
    if (_gObject) {
        ((GraphicalReaction*)_gObject)->updateValues(_mw);
        showInfo(_gObject);
    }
}

// Species Reference Feature Menu

SpeciesReferenceFeatureMenu::SpeciesReferenceFeatureMenu(QWidget* parent, MainWindow* mw) : ItemFeatureMenuElement(parent, mw) {
    headFMenu = NULL;
    titleLabel.setText("Species Reference");
    
    /// model features
    modelBranch = new MyGroupBox(tabMenu);
    // id
    modelContentLayout.addWidget(new MyLabel("Id:"), modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    // reaction
    modelContentLayout.addWidget(new MyLabel("Reaction:"), modelContentLayout.rowCount(), 0);
    reactionIdLineEdit.setReadOnly(true);
    modelContentLayout.addWidget(&reactionIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    // species
    modelContentLayout.addWidget(new MyLabel("Species:"), modelContentLayout.rowCount(), 0);
    speciesIdLineEdit.setReadOnly(true);
    modelContentLayout.addWidget(&speciesIdLineEdit, modelContentLayout.rowCount() - 1, 1);
    // role
    modelContentLayout.addWidget(new MyLabel("Role:"), modelContentLayout.rowCount(), 0);
    roleLineEdit.setReadOnly(true);
    modelContentLayout.addWidget(&roleLineEdit, modelContentLayout.rowCount() - 1, 1);
    modelContentLayout.setAlignment(Qt::AlignTop);
    modelBranch->setLayout(&modelContentLayout);
    
    /// layout features
    layoutBranch = new MyGroupBox(tabMenu);
    // id
    layoutContentLayout.addWidget(new MyLabel("Layout Id:"), layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdLineEdit, layoutContentLayout.rowCount() - 1, 1);
    layoutContentLayout.addItem(new QSpacerItem(0, 20), layoutContentLayout.rowCount(), 0);
    // curve
    layoutContentLayout.addWidget(new MyLabel("Curve"), layoutContentLayout.rowCount(), 0);
    layoutContentLayout.addWidget(&curveAddOrRemoveButton, layoutContentLayout.rowCount() - 1, 1, Qt::AlignRight);
    curveFMenu = new CurveFeatureMenu(layoutBranch, _mw);
    connect(curveFMenu, SIGNAL(curveChanged()), SLOT(updateValues()));
    layoutContentLayout.addWidget(curveFMenu, layoutContentLayout.rowCount(), 0, 1, 2);
    layoutContentLayout.setAlignment(Qt::AlignTop);
    layoutBranch->setLayout(&layoutContentLayout);
    
    /// render features
    renderStyleBranch = new MyGroupBox(tabMenu);
    renderTreeView = new MyTreeView(renderStyleBranch);
    // style
    renderStyleContentLayout.addWidget(new MyLabel("Style"), renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderStyleContentLayout.addWidget(&styleAddOrRemoveButton, renderStyleContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    // stroke
    strokeFMenu = new StrokeFeatureMenu(renderTreeView, _mw);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    renderTreeView->addBranchWidget(strokeFMenu, "Stroke");
    // heads
    headFMenu = new HeadFeatureMenu(renderTreeView, _mw);
    connect(headFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    connect(_mw, SIGNAL(numberOfGLEndingChanged()), headFMenu, SLOT(updateValues()));
    renderTreeView->addBranchWidget(headFMenu, "Heads");
    renderStyleContentLayout.addWidget(renderTreeView, renderStyleContentLayout.rowCount(), 0, 1, 2);
    renderStyleBranch->setLayout(&renderStyleContentLayout);
    
    /// tab menu
    tabMenu->addTab(modelBranch, "Model");
    tabMenu->addTab(layoutBranch, "Layout");
    tabMenu->addTab(renderStyleBranch, "Render");
    //tabMenu->addTab(textBranch, "Text");
    qLayout.addItem(new QSpacerItem(0, 10), qLayout.rowCount(), 0);
    qLayout.addWidget(tabMenu, qLayout.rowCount(), 0);
    
    // reset values;
    resetValues();
}

void SpeciesReferenceFeatureMenu::showInfo(GraphicalObjectBase* gObject) {
    // reset values
    resetValues();
    
    if (gObject) {
        _gObject = gObject;
        
        if (_gObject->isSetNGraphicalObject()) {
            _gO = _gObject->getNGraphicalObject();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLineEdit.setText(ne_ne_getId(_gO).c_str());
            // set reaction
            if (ne_ne_isSetId(ne_sr_getReaction((NSpeciesReference*)_gO)))
                reactionIdLineEdit.setText(ne_ne_getId(ne_sr_getReaction((NSpeciesReference*)_gO)).c_str());
            // set species
            if (ne_ne_isSetId(ne_sr_getSpecies((NSpeciesReference*)_gO)))
                speciesIdLineEdit.setText(ne_ne_getId(ne_sr_getSpecies((NSpeciesReference*)_gO)).c_str());
            // set role
            if (ne_sr_isSetRole((NSpeciesReference*)_gO))
                roleLineEdit.setText(ne_sr_getRoleAsString((NSpeciesReference*)_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdLineEdit.setText(ne_go_getGlyphId(_gO).c_str());
            // set curve
            if (ne_sr_isSetCurve((NSpeciesReference*)_gO)) {
                curveFMenu->showInfo(ne_sr_getCurve((NSpeciesReference*)_gO));
                curveAddOrRemoveButton.enableRemove();
            }
            
            /// render features
            if (_gObject->isSetStyle()) {
                _style = _gObject->getStyle();
                styleAddOrRemoveButton.enableRemove();
                if (renderTreeView)
                    renderTreeView->showColumn(0);
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                // head features
                headFMenu->showInfo(_gO, _style);
            }
        }
    }
    
    show();
}

void SpeciesReferenceFeatureMenu::hideInfo() {
    FeatureMenuElement::hideInfo();
    
    if (headFMenu)
        headFMenu->collapseTree();
}

void SpeciesReferenceFeatureMenu::resetValues() {
    modelIdLineEdit.setText("N/A");
    reactionIdLineEdit.setText("N/A");
    speciesIdLineEdit.setText("N/A");
    roleLineEdit.setText("N/A");
    layoutIdLineEdit.clear();
    curveAddOrRemoveButton.resetValues();
    if (curveFMenu)
        curveFMenu->resetValues();
    styleAddOrRemoveButton.resetValues();
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (headFMenu)
        headFMenu->resetValues();
    if (renderTreeView)
        renderTreeView->hideColumn(0);
    
    _gObject = NULL;
    _gO = NULL;
    _style = NULL;
}

void SpeciesReferenceFeatureMenu::updateValues() {
    if (_gObject) {
        ((GraphicalSReference*)_gObject)->updateValues(_mw);
        showInfo(_gObject);
    }
}

// Line Ending Feature Menu

LineEndingFeatureMenu::LineEndingFeatureMenu(QWidget* parent, MainWindow* mw) : ItemFeatureMenuElement(parent, mw) {
    layoutBranch = NULL;
    renderStyleBranch = NULL;
    renderGeometricShapeBranch = NULL;
    textBranch = NULL;
    titleLabel.setText("Line Ending");
    
    /// layout features
    layoutBranch = new MyGroupBox(tabMenu);
    // bounding box
    layoutContentLayout.addWidget(new MyLabel("Bounding Box"), layoutContentLayout.rowCount(), 0);
    bBoxFMenu = new BoundingBoxFeatureMenu(layoutBranch, _mw);
    connect(bBoxFMenu, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(updateLineEnding(VLineEnding*)));
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 2);
    layoutContentLayout.setAlignment(Qt::AlignTop);
    layoutBranch->setLayout(&layoutContentLayout);
    
    /// render features
    renderStyleBranch = new MyGroupBox(tabMenu);
    renderTreeView = new MyTreeView(renderStyleBranch);
    // id
    renderStyleContentLayout.addWidget(new MyLabel("Render Id:"), renderStyleContentLayout.rowCount() - 1, 0);
    renderStyleContentLayout.addWidget(&renderIdLineEdit, renderStyleContentLayout.rowCount() - 1, 1);
    connect(&renderIdLineEdit, SIGNAL(editingFinished()), this, SLOT(changeRenderId()));
    // enable rotation
    renderStyleContentLayout.addWidget(new MyLabel("Enable Rotation:"), renderStyleContentLayout.rowCount(), 0);
    renderStyleContentLayout.addWidget(&enableRotationButton, renderStyleContentLayout.rowCount() - 1, 1);
    connect(&enableRotationButton, SIGNAL(clicked()), this, SLOT(changeEnableRotation()));
    // fill
    fillFMenu = new FillFeatureMenu(renderTreeView, _mw);
    connect(fillFMenu, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(updateLineEnding(VLineEnding*)));
    renderTreeView->addBranchWidget(fillFMenu, "Fill");
    // geometric shape
    renderGeometricShapeBranch = new MyGroupBox(renderTreeView);
    renderGeometricShapeContentLayout.addWidget(&addRemoveGeometricShapeButtons, renderGeometricShapeContentLayout.rowCount(), 2);
    connect(&addRemoveGeometricShapeButtons, SIGNAL(geometricShapeChosen(const QString&)), this, SLOT(addGeometricShape(const QString&)));
    connect(&addRemoveGeometricShapeButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeGeometricShape(const unsigned int&)));
    renderGeometricShapeBranch->setLayout(&renderGeometricShapeContentLayout);
    renderTreeView->addBranchWidget(renderGeometricShapeBranch, "Geometric Shapes");
    renderStyleContentLayout.addWidget(renderTreeView, renderStyleContentLayout.rowCount(), 0, 1, 2);
    renderStyleBranch->setLayout(&renderStyleContentLayout);
    
    /// tab menu
    tabMenu->addTab(layoutBranch, "Layout");
    tabMenu->addTab(renderStyleBranch, "Render");
    qLayout.addItem(new QSpacerItem(0, 10), qLayout.rowCount(), 0);
    qLayout.addWidget(tabMenu, qLayout.rowCount(), 0);
    
    // reset values;
    resetValues();
}

void LineEndingFeatureMenu::showInfo(GraphicalObjectBase* gObject, GraphicalSReference* gSReference, QString head) {
    // reset values
    resetValues();
    
    // render features
    if (gObject && gSReference) {
        _gObject = gObject;
        _gSReference = gSReference;
        _head = head;
        
        if (((GraphicalLineEnding*)_gObject)->isSetLEnding()) {
            _lE = ((GraphicalLineEnding*)_gObject)->getLEnding();
            // render features
            // set id
            if (ne_ve_isSetId(_lE))
                renderIdLineEdit.setText(ne_ve_getId(_lE).c_str());
            // set enable rotation
            if (ne_le_isSetEnableRotation(_lE) && !ne_le_getEnableRotation(_lE)) {
                enableRotationButton.setText("False");
            }
            else
                enableRotationButton.setText("True");
            
            if (ne_le_isSetBoundingBox(_lE)) {
                if (renderTreeView)
                    renderTreeView->showColumn(0);
                // bounding box
                bBoxFMenu->showInfo(_lE);
                // fill features
                fillFMenu->showInfo(_lE);
                // geometric shape features
                if (ne_grp_getNumGeometricShapes(ne_le_getGroup(_lE)))
                    addRemoveGeometricShapeButtons.enableRemoveButton(ne_le_getGroup(_lE));
                GeometricShapeFeatureMenuElement* gSFMenuElement = NULL;
                for (int i = 0; i < ne_grp_getNumGeometricShapes(ne_le_getGroup(_lE)); ++i) {
                    gSFMenuElement = NULL;
                    // set geometric shape feature menu
                    switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(_lE), i))) {
                        case 0:
                            gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                            renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Image", "Geometric Shapes");
                            ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE, i);
                            break;
                            
                        case 1:
                            gSFMenuElement = new RenderCurveGeometricShapeFeatureMenu(this, _mw, true);
                            renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": RenderCurve", "Geometric Shapes");
                            ((RenderCurveGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE, i);
                            break;
                            
                        //case 2:
                            //gSFMenuElement = new TextGeometricShapeFeatureMenu(this, _mw);
                            //break;
                            
                        case 3:
                            gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw, true);
                            renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Rectangle", "Geometric Shapes");
                            ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE, i);
                            break;
                            
                        case 4:
                            gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw, true);
                            renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Ellipse", "Geometric Shapes");
                            ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE, i);
                            break;
                            
                        case 5:
                            gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw, false, true);
                            renderTreeView->addBranchWidget(gSFMenuElement, QString::number(i + 1) + ": Polygon", "Geometric Shapes");
                            ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE, i);
                            break;
                            
                        default:
                            break;
                    }
                    
                    if (gSFMenuElement) {
                        _gSFMenuElements.push_back(gSFMenuElement);
                        connect(gSFMenuElement, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(updateLineEnding(VLineEnding*)));
                    }
                }
            }
        }
    }
    
    show();
}

void LineEndingFeatureMenu::resetValues() {
    renderIdLineEdit.clear();
    enableRotationButton.setText("N/A");
    if (bBoxFMenu)
        bBoxFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    clearGSFMenuElements();
    if (renderTreeView) {
        renderTreeView->removeBranches("Geometric Shapes", 1);
        renderTreeView->hideColumn(0);
    }
    addRemoveGeometricShapeButtons.resetValues();
    
    _gObject = NULL;
    _gSReference = NULL;
    _lE = NULL;
    _head.clear();
}

void LineEndingFeatureMenu::updateValues() {
    if (_gObject && _gSReference)
        showInfo(_gObject, _gSReference, _head);
}

void LineEndingFeatureMenu::updateLineEnding(VLineEnding* lineEnding) {
    if (_gObject && ((GraphicalLineEnding*)_gObject)->isSetLEnding() && _gSReference && lineEnding) {
        if (!stringCompare(ne_ve_getId(((GraphicalLineEnding*)_gObject)->getLEnding()), ne_ve_getId(lineEnding))) {
            // create a new graphical line ending
            _gObject = new GraphicalLineEnding();
            
            // set the line endding of graphical line ending
            ((GraphicalLineEnding*)_gObject)->setLEnding(lineEnding);
            
            // set the id of graphical line ending
            _gObject->setId(ne_ve_getId(lineEnding));
            
            // store the graphical line ending
            _mw->addGLEnding(((GraphicalLineEnding*)_gObject));
            
            // get graphical line ending values
            ((GraphicalLineEnding*)_gObject)->updateValues(_mw);
            
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gSReference->getNGraphicalObject(), _gSReference->getStyle());
            
            if (_head == "start")
                ne_grp_setStartHead(ne_stl_getGroup(style), ne_ve_getId(lineEnding));
            else if (_head == "end")
                ne_grp_setEndHead(ne_stl_getGroup(style), ne_ve_getId(lineEnding));
            
            if (style) {
                _gSReference->setStyle(style);
                _gSReference->updateValues(_mw);
            }
        }
        else {
            ((GraphicalLineEnding*)_gObject)->updateValues(_mw);
            _gSReference->updateValues(_mw);
        }
        
        updateValues();
    }
}

void LineEndingFeatureMenu::changeRenderId() {
    if (_mw && _mw->isSetSBMLDocument() && !stringCompare(renderIdLineEdit.text().toStdString(), ne_ve_getId(_lE))) {
        VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
        
        if (!ne_ve_setId(_mw->getSBMLDocument()->getVeneer(), lineEnding, renderIdLineEdit.text().toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            updateLineEnding(lineEnding);
        }
    }
}

void LineEndingFeatureMenu::changeEnableRotation() {
    if (_mw && _mw->isSetSBMLDocument()) {
        VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
        
        if (lineEnding) {
            if (enableRotationButton.text() == "True" && !ne_le_enableRotation(lineEnding, false))
                enableRotationButton.setText("False");
            else if (enableRotationButton.text() == "False" && !ne_le_enableRotation(lineEnding, true))
                enableRotationButton.setText("True");
            
            _mw->getSBMLDocument()->setRenderModified(true);
            updateLineEnding(lineEnding);
        }
    }
}

// BoundingBoxFeatureMenu

BoundingBoxFeatureMenu::BoundingBoxFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    bBoxTreeView = NULL;
    bBoxPositionBranch = NULL;
    bBoxDimensionsBranch = NULL;
    setLayout(&bBoxLayout);
    
    /// features
    bBoxTreeView = new MyTreeView(parent);
    // position
    bBoxPositionBranch = new MyGroupBox(bBoxTreeView);
    positionDualSpinBox.setType("Position");
    bBoxPositionLayout.addWidget(&positionDualSpinBox, bBoxPositionLayout.rowCount() - 1, 0);
    connect(&positionDualSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePosition(const double&, const double&)));
    bBoxPositionBranch->setLayout(&bBoxPositionLayout);
    bBoxTreeView->addBranchWidget(bBoxPositionBranch, "Position");
    // dimensions
    bBoxDimensionsBranch = new MyGroupBox(bBoxTreeView);
    dimensionsDualSpinBox.setType("Dimensions");
    bBoxDimensionsLayout.addWidget(&dimensionsDualSpinBox, bBoxDimensionsLayout.rowCount() - 1, 0);
    connect(&dimensionsDualSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensions(const double&, const double&)));
    bBoxDimensionsBranch->setLayout(&bBoxDimensionsLayout);
    bBoxTreeView->addBranchWidget(bBoxDimensionsBranch, "Dimensions");
    bBoxLayout.addWidget(bBoxTreeView, bBoxLayout.rowCount(), 0, 1, 2);
    
    // reset values
    resetValues();
}

void BoundingBoxFeatureMenu::showInfo(NGraphicalObject* gO) {
    resetValues();
    if (gO) {
        _gO = gO;
        
        if (ne_go_isSetBoundingBox(_gO)) {
            // set position
            positionDualSpinBox.setRange(_mw);
            positionDualSpinBox.setCurrentValue(ne_bb_getX(ne_go_getBoundingBox(_gO)), ne_bb_getY(ne_go_getBoundingBox(_gO)));
            // set dimensions
            dimensionsDualSpinBox.setRange(_mw);
            dimensionsDualSpinBox.setCurrentValue(ne_bb_getWidth(ne_go_getBoundingBox(_gO)), ne_bb_getHeight(ne_go_getBoundingBox(_gO)));
            
            if (bBoxTreeView)
                bBoxTreeView->showColumn(0);
        }
    }
}

void BoundingBoxFeatureMenu::showInfo(VLineEnding* lE) {
    resetValues();
    if (lE) {
        _lE = lE;
        
        if (ne_le_isSetBoundingBox(_lE)) {
            // set position
            positionDualSpinBox.setRange(_mw);
            positionDualSpinBox.setCurrentValue(ne_bb_getX(ne_le_getBoundingBox(_lE)), ne_bb_getY(ne_le_getBoundingBox(_lE)));
            // set dimensions
            dimensionsDualSpinBox.setRange(_mw);
            dimensionsDualSpinBox.setCurrentValue(ne_bb_getWidth(ne_le_getBoundingBox(_lE)), ne_bb_getHeight(ne_le_getBoundingBox(_lE)));
            
            if (bBoxTreeView)
                bBoxTreeView->showColumn(0);
        }
    }
}

void BoundingBoxFeatureMenu::resetValues() {
    positionDualSpinBox.resetValues();
    dimensionsDualSpinBox.resetValues();
    if (bBoxTreeView)
        bBoxTreeView->hideColumn(0);
    
    _gO = NULL;
    _lE = NULL;
}

void BoundingBoxFeatureMenu::collapseTree() {
    if (bBoxTreeView)
        bBoxTreeView->collapseAll();
}

void BoundingBoxFeatureMenu::changePosition(const double& x, const double& y) {
    if (_mw && _mw->isSetSBMLDocument()) {
        if (_gO) {
            ne_bb_setX(ne_go_getBoundingBox(_gO), x);
            ne_bb_setY(ne_go_getBoundingBox(_gO), y);
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit bBoxPositionChanged();
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            ne_bb_setX(ne_le_getBoundingBox(lineEnding), x);
            ne_bb_setY(ne_le_getBoundingBox(lineEnding), y);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

void BoundingBoxFeatureMenu::changeDimensions(const double& width, const double& height) {
    if (_mw && _mw->isSetSBMLDocument()) {
        if (_gO) {
            ne_bb_setWidth(ne_go_getBoundingBox(_gO), width);
            ne_bb_setHeight(ne_go_getBoundingBox(_gO), height);
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit bBoxDimensionsChanged();
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            ne_bb_setWidth(ne_le_getBoundingBox(lineEnding), width);
            ne_bb_setHeight(ne_le_getBoundingBox(lineEnding), height);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

// CurveFeatureMenu

CurveFeatureMenu::CurveFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    curveTreeView = NULL;
    setLayout(&curveLayout);
    
    /// features
    curveTreeView = new MyTreeView(parent);
    // segments
    curveSegmentsBranch = new MyGroupBox(curveTreeView);
    curveSegmentsLayout.addWidget(&addRemoveSegmentButtons, curveSegmentsLayout.rowCount(), 2);
    connect(&addRemoveSegmentButtons, SIGNAL(addItemFeaturesChosen(const unsigned int&, const double&, const double&, const double&, const double&)), this, SLOT(addLine(const unsigned int&, const double&, const double&, const double&, const double&)));
    connect(&addRemoveSegmentButtons, SIGNAL(addItemFeaturesChosen(const unsigned int&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&)), this, SLOT(addCubicBezier(const unsigned int&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&)));
    connect(&addRemoveSegmentButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeLine(const unsigned int&)));
    curveSegmentsBranch->setLayout(&curveSegmentsLayout);
    curveTreeView->addBranchWidget(curveSegmentsBranch, "Segments");
    curveLayout.addWidget(curveTreeView, curveLayout.rowCount(), 0, 1, 3);
    
    // reset values
    resetValues();
    
    
    
    
    /*
    
    
    _mw = mw;
    _parent = parent;
    curveTreeView = NULL;
    
    if (parentLayout) {
        /// features
        curveTreeView = new MyTreeView(parent);
        // segments
        curveSegmentsBranch = new MyGroupBox(curveTreeView);
        curveSegmentsLayout.addWidget(&addRemoveSegmentButtons, curveSegmentsLayout.rowCount(), 2);
        connect(&addRemoveSegmentButtons, SIGNAL(addItemFeaturesChosen(const unsigned int&, const double&, const double&, const double&, const double&)), this, SLOT(addLine(const unsigned int&, const double&, const double&, const double&, const double&)));
        connect(&addRemoveSegmentButtons, SIGNAL(addItemFeaturesChosen(const unsigned int&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&)), this, SLOT(addCubicBezier(const unsigned int&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&)));
        connect(&addRemoveSegmentButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removeLine(const unsigned int&)));
        curveSegmentsBranch->setLayout(&curveSegmentsLayout);
        curveTreeView->addBranchWidget(curveSegmentsBranch, "Segments");
        parentLayout->addWidget(curveTreeView, parentLayout->rowCount(), 0, 1, 3);
        
        // reset values
        resetValues();
    }
     */
}

void CurveFeatureMenu::showInfo(LCurve* curve) {
    resetValues();
    if(curve) {
        _curve = curve;
        LineFeatureMenu* lFeatureMenu = NULL;
        for (int i = 0; i < ne_crv_getNumElements(_curve); ++i) {
            if (ne_ls_isCubicBezier(ne_crv_getElement(_curve, i)))
                lFeatureMenu = new CubicBezierFeatureMenu(_parent, _mw);
            else
                lFeatureMenu = new LineFeatureMenu(_parent, _mw);
            connect(lFeatureMenu, SIGNAL(lineChanged()), this, SIGNAL(curveChanged()));
            lFeatureMenu->showInfo(ne_crv_getElement(_curve, i));
            _lineFeatureMenus.push_back(lFeatureMenu);
            curveTreeView->addBranchWidget(lFeatureMenu, "Segment " + QString::number(i + 1), "Segments");
        }
        addRemoveSegmentButtons.enableAddButton(_mw, _lineFeatureMenus.size());
        if (_lineFeatureMenus.size() > 1)
            addRemoveSegmentButtons.enableRemoveButton(_lineFeatureMenus.size());
        
        if (curveTreeView)
            curveTreeView->showColumn(0);
    }
    
    show();
}

void CurveFeatureMenu::resetValues() {
    hide();
    for (constLineFeatureMenuIt lFMIt = lineFeatureMenusBegin(); lFMIt != lineFeatureMenusEnd(); ++ lFMIt) {
        (*lFMIt)->resetValues();
        curveLayout.removeWidget(*lFMIt);
        (*lFMIt)->deleteLater();
    }
    _lineFeatureMenus.clear();
    if (curveTreeView) {
        curveTreeView->removeBranches("Segments", 1);
        curveTreeView->hideColumn(0);
    }
    addRemoveSegmentButtons.resetValues();
    
    _curve = NULL;
}

void CurveFeatureMenu::collapseTree() {
    if (curveTreeView)
        curveTreeView->collapseAll();
    
    for (constLineFeatureMenuIt lFMIt = lineFeatureMenusBegin(); lFMIt != lineFeatureMenusEnd(); ++ lFMIt)
        (*lFMIt)->collapseTree();
}

void CurveFeatureMenu::addLine(const unsigned int& itemIndex, const double& itemPointStartXValue, const double& itemPointStartYValue, const double& itemPointEndXValue, const double& itemPointEndYValue) {
    if (_mw && _mw->isSetSBMLDocument() && _curve) {
        LPoint* startPoint = new LPoint(itemPointStartXValue, itemPointStartYValue);
        LPoint* endPoint = new LPoint(itemPointEndXValue, itemPointEndYValue);
        if (ne_crv_addElement(_curve, itemIndex, startPoint, endPoint)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit curveChanged();
        }
    }
}

void CurveFeatureMenu::addCubicBezier(const unsigned int& itemIndex, const double& itemCubicBezierStartXValue, const double& itemCubicBezierStartYValue, const double& itemCubicBezierEndXValue, const double& itemCubicBezierEndYValue, const double& itemCubicBezierBasePoint1XValue, const double& itemCubicBezierBasePoint1YValue, const double& itemCubicBezierBasePoint2XValue, const double& itemCubicBezierBasePoint2YValue) {
    if (_mw && _mw->isSetSBMLDocument() && _curve) {
        LPoint* startPoint = new LPoint(itemCubicBezierStartXValue, itemCubicBezierStartYValue);
        LPoint* endPoint = new LPoint(itemCubicBezierEndXValue, itemCubicBezierEndYValue);
        LPoint* basePoint1 = new LPoint(itemCubicBezierBasePoint1XValue, itemCubicBezierBasePoint1YValue);
        LPoint* basePoint2 = new LPoint(itemCubicBezierBasePoint2XValue, itemCubicBezierBasePoint2YValue);
        if (ne_crv_addElement(_curve, itemIndex, startPoint, endPoint, basePoint1, basePoint2)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit curveChanged();
        }
    }
}

void CurveFeatureMenu::removeLine(const unsigned int& itemIndex) {
    if (_mw && _mw->isSetSBMLDocument() && !ne_crv_removeElement(_curve, itemIndex)) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit curveChanged();
    }
}

// StrokeFeatureMenu

StrokeFeatureMenu::StrokeFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    setLayout(&strokeLayout);
    
    // stroke color
    strokeLayout.addWidget(new MyLabel("Color:"), strokeLayout.rowCount(), 0);
    strokeColorPickerButton= new MyColorPickerButton(_mw);
    strokeLayout.addWidget(strokeColorPickerButton, strokeLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(strokeColorPickerButton, SIGNAL(colorChosen(const QString&, const QString&)), this, SLOT(changeStrokeColor(const QString&, const QString&)));
    // stroke width
    strokeLayout.addWidget(new MyLabel("Width:"), strokeLayout.rowCount(), 0);
    strokeWidthSpinBox.setFixedWidth(60);
    strokeLayout.addWidget(&strokeWidthSpinBox, strokeLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&strokeWidthSpinBox, SIGNAL(editingFinished()), this, SLOT(changeStrokeWidth()));
    // stroke dash array
    strokeLayout.addWidget(new MyLabel("DashArray:"), strokeLayout.rowCount(), 0);
    strokedashArrayComboBox.setFixedWidth(120);
    strokeLayout.addWidget(&strokedashArrayComboBox, strokeLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&strokedashArrayComboBox, SIGNAL(dashArrayChosen(std::vector<unsigned int>*)), this, SLOT(changeStrokeDashArray(std::vector<unsigned int>*)));
    
    // reset values
    resetValues();
}

void StrokeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const int& gSIndex) {
    if (gO && style) {
        _style = style;
        _gO = gO;
        _gSIndex = gSIndex;
        
        if (ne_go_getType(_gO) == 0)
            strokeWidthSpinBox.setRange(0, 20);
        else
            strokeWidthSpinBox.setRange(0, 10);
        
        // from geometric shape
        if (_gSIndex != -1) {
            // set stroke color
            if (ne_gs_isSetStrokeColor(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)))
                strokeColorPickerButton->setBackgroundColor(ne_gs_getStrokeColor(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)).c_str());
            // set stroke width
            if (ne_gs_isSetStrokeWidth(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)))
                strokeWidthSpinBox.setValue(ne_gs_getStrokeWidth(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)));
            // set stroke dash array
            if (ne_gs_isSetStrokeDashArray(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)))
                strokedashArrayComboBox.setDashArray(ne_gs_getStrokeDashArray(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)));
        }
        
        // from render group
        else {
            // set stroke color
            if (ne_grp_isSetStrokeColor(ne_stl_getGroup(_style)))
                strokeColorPickerButton->setBackgroundColor(ne_grp_getStrokeColor(ne_stl_getGroup(_style)).c_str());
            // set stroke width
            if (ne_grp_isSetStrokeWidth(ne_stl_getGroup(_style)))
                strokeWidthSpinBox.setValue(ne_grp_getStrokeWidth(ne_stl_getGroup(_style)));
            // set stroke dash array
            if (ne_grp_isSetStrokeDashArray(ne_stl_getGroup(_style)))
                strokedashArrayComboBox.setDashArray(ne_grp_getStrokeDashArray(ne_stl_getGroup(_style)));
        }
    }
}

void StrokeFeatureMenu::resetValues() {
    strokeColorPickerButton->setBackgroundColor("White");
    strokeWidthSpinBox.setValue(0);
    strokedashArrayComboBox.resetValues();
    _gO = NULL;
    _style = NULL;
    _gSIndex = -1;
}

void StrokeFeatureMenu::changeStrokeColor(const QString& color, const QString& value) {
    if (!ne_ven_getColor(_mw->getSBMLDocument()->getVeneer(), -1, color.toStdString())) {
        VColorDefinition* c = ne_ven_addNewColor(_mw->getSBMLDocument()->getVeneer(), color.toStdString(), value.toStdString());
        
        // add the color to list of colors
        if (c) {
            _mw->getSBMLDocument()->setRenderModified(true);
            // create a new graphical color
            GraphicalColor* _gColor = new GraphicalColor();
            // get info from veneer color
            getInfoFromColor(_gColor, c);
            // store the graphical color
            _mw->addGColor(_gColor);
        }
    }
    
    if (_gO && _style) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (_gSIndex != -1)
            ne_gs_setStrokeColor(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), color.toStdString());
        else
            ne_grp_setStrokeColor(ne_stl_getGroup(style), color.toStdString());
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

void StrokeFeatureMenu::changeStrokeWidth() {
    if (_mw && _mw->isSetSBMLDocument() && strokeWidthSpinBox.value() != 0) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (_gSIndex != -1)
            ne_gs_setStrokeWidth(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), strokeWidthSpinBox.value());
        else
            ne_grp_setStrokeWidth(ne_stl_getGroup(style), strokeWidthSpinBox.value());
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

void StrokeFeatureMenu::changeStrokeDashArray(std::vector<unsigned int>* dashArrayVector) {
    if (_mw && _mw->isSetSBMLDocument() && dashArrayVector) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (_gSIndex != -1)
            ne_gs_setStrokeDashArray(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), dashArrayVector);
        else
            ne_grp_setStrokeDashArray(ne_stl_getGroup(style), dashArrayVector);
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

// FillFeatureMenu

FillFeatureMenu::FillFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    setLayout(&fillLayout);
    
    // fill color label
    fillLayout.addWidget(new MyLabel("Color:"), fillLayout.rowCount(), 0);
    // fill color
    fillColorPickerButton = new MyColorPickerButton(_mw);
    fillLayout.addWidget(fillColorPickerButton, fillLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(fillColorPickerButton, SIGNAL(colorChosen(const QString&, const QString&)), this, SLOT(changeFillColor(const QString&, const QString&)));
    
    // reset values
    resetValues();
}

void FillFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const int& gSIndex) {
    if (gO && style) {
        _style = style;
        _gO = gO;
        _gSIndex = gSIndex;
        
        if (ne_go_isSetBoundingBox(_gO)) {
            // set fill color
            if (_gSIndex != -1) {
                if (ne_gs_isSetFillColor(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)))
                    fillColorPickerButton->setBackgroundColor(ne_gs_getFillColor(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)).c_str());
            }
            else if (ne_grp_isSetFillColor(ne_stl_getGroup(_style)))
                fillColorPickerButton->setBackgroundColor(ne_grp_getFillColor(ne_stl_getGroup(_style)).c_str());
        }
    }
}

void FillFeatureMenu::showInfo(VLineEnding* lE) {
    if (lE) {
        _lE = lE;
        
        if (ne_le_isSetBoundingBox(_lE) && ne_grp_isSetFillColor(ne_le_getGroup(_lE)))
            fillColorPickerButton->setBackgroundColor(ne_grp_getFillColor(ne_le_getGroup(_lE)).c_str());
    }
}

void FillFeatureMenu::resetValues() {
    //hide();
    fillColorPickerButton->setBackgroundColor("White");
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gSIndex = -1;
}

void FillFeatureMenu::changeFillColor(const QString& color, const QString& value) {
    if (!ne_ven_getColor(_mw->getSBMLDocument()->getVeneer(), -1, color.toStdString())) {
        VColorDefinition* c = ne_ven_addNewColor(_mw->getSBMLDocument()->getVeneer(), color.toStdString(), value.toStdString());
        
        // add the color to list of colors
        if (c) {
            _mw->getSBMLDocument()->setRenderModified(true);
            // create a new graphical color
            GraphicalColor* _gColor = new GraphicalColor();
            // get info from veneer color
            getInfoFromColor(_gColor, c);
            // store the graphical color
            _mw->addGColor(_gColor);
        }
    }
    
    if (_gO && _style) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (_gSIndex != -1)
            ne_gs_setFillColor(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), color.toStdString());
        else
            ne_grp_setFillColor(ne_stl_getGroup(style), color.toStdString());
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
    else if (_lE) {
        VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
        
        if (!ne_grp_setFillColor(ne_le_getGroup(lineEnding), color.toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

// HeadFeatureMenu

HeadFeatureMenu::HeadFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    headTreeView = NULL;
    headStartBranch = NULL;
    headEndBranch = NULL;
    setLayout(&headLayout);
    
    /// features
    headTreeView = new MyTreeView(parent);
    // start
    headStartBranch = new MyGroupBox(headTreeView);
    headStartLayout.addWidget(new MyLabel("Start:"), headStartLayout.rowCount(), 0);
    headStartLayout.addWidget(&startHeadPickerComboBox, headStartLayout.rowCount() - 1, 1);
    connect(&startHeadPickerComboBox, SIGNAL(headChosen(const QString&)), this, SLOT(changeStartHead(const QString&)));
    headStartBranch->setLayout(&headStartLayout);
    headTreeView->addBranchWidget(headStartBranch, "Start");
    // end
    headEndBranch = new MyGroupBox(headTreeView);
    headEndLayout.addWidget(new MyLabel("End:"), headEndLayout.rowCount(), 0);
    headEndLayout.addWidget(&endHeadPickerComboBox, headEndLayout.rowCount() - 1, 1);
    connect(&endHeadPickerComboBox, SIGNAL(headChosen(const QString&)), this, SLOT(changeEndHead(const QString&)));
    headEndBranch->setLayout(&headEndLayout);
    headTreeView->addBranchWidget(headEndBranch, "End");
    headLayout.addWidget(headTreeView, headLayout.rowCount() - 1, 1);

    // reset values
    resetValues();
}

void HeadFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const int& gSIndex) {
    if (gO && style) {
        _style = style;
        _gO = gO;
        _gSIndex = gSIndex;
        
        if (_mw && _mw->getNumGLEndings()) {
            startHeadPickerComboBox.enableItems(_mw);
            endHeadPickerComboBox.enableItems(_mw);
            
            for (MainWindow::constGLEndingIt gLEIt = _mw->gLEndingsBegin(); gLEIt != _mw->gLEndingsEnd(); ++gLEIt) {
                if ((*gLEIt)->isSetId()) {
                    // from geometric shape
                    if (_gSIndex != -1) {
                        // start head
                        if (ne_rc_isSetStartHead(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)) && stringCompare((*gLEIt)->getId(), ne_rc_getStartHead(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex))))
                            startHeadPickerComboBox.setHead(ne_rc_getStartHead(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)).c_str());
                        // end head
                        if (ne_rc_isSetEndHead(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)) && stringCompare((*gLEIt)->getId(), ne_rc_getEndHead(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex))))
                        endHeadPickerComboBox.setHead(ne_rc_getEndHead(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex)).c_str());
                    }
                    // from rendergroup
                    else {
                        // start head
                        if (ne_grp_isSetStartHead(ne_stl_getGroup(_style)) && stringCompare((*gLEIt)->getId(), ne_grp_getStartHead(ne_stl_getGroup(_style))))
                            startHeadPickerComboBox.setHead(ne_grp_getStartHead(ne_stl_getGroup(_style)).c_str());
                        // end head
                        if (ne_grp_isSetEndHead(ne_stl_getGroup(_style)) && stringCompare((*gLEIt)->getId(), ne_grp_getEndHead(ne_stl_getGroup(_style))))
                            endHeadPickerComboBox.setHead(ne_grp_getEndHead(ne_stl_getGroup(_style)).c_str());
                    }
                }
            }
        }
    }
}

void HeadFeatureMenu::showInfo(VLineEnding* lE) {
    if (lE) {
        _lE = lE;
        
        if (_mw && _mw->getNumGLEndings()) {
            startHeadPickerComboBox.enableItems(_mw);
            endHeadPickerComboBox.enableItems(_mw);
            
            for (MainWindow::constGLEndingIt gLEIt = _mw->gLEndingsBegin(); gLEIt != _mw->gLEndingsEnd(); ++gLEIt) {
                if ((*gLEIt)->isSetId()) {
                    // start head
                    if (ne_grp_isSetStartHead(ne_le_getGroup(_lE)) && stringCompare((*gLEIt)->getId(), ne_grp_getStartHead(ne_le_getGroup(_lE))))
                        startHeadPickerComboBox.setHead(ne_grp_getStartHead(ne_le_getGroup(_lE)).c_str());
                    // end head
                    if (ne_grp_isSetEndHead(ne_le_getGroup(_lE)) && stringCompare((*gLEIt)->getId(), ne_grp_getEndHead(ne_le_getGroup(_lE))))
                        endHeadPickerComboBox.setHead(ne_grp_getEndHead(ne_le_getGroup(_lE)).c_str());
                }
            }
        }
    }
}

void HeadFeatureMenu::resetValues() {
    startHeadPickerComboBox.resetValues();
    endHeadPickerComboBox.resetValues();
    
    _gO = NULL;
    _style = NULL;
    _gSIndex = -1;
}

void HeadFeatureMenu::collapseTree() {
    //if (headTreeView)
        //headTreeView->collapseAll();
}

void HeadFeatureMenu::updateValues() {
    if (_gO && _style)
        showInfo(_gO, _style);
}

void HeadFeatureMenu::changeStartHead(const QString& head) {
    if (_mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if(!ne_grp_setStartHead(ne_stl_getGroup(style), head.toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

void HeadFeatureMenu::changeEndHead(const QString& head) {
    if (_mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if(!ne_grp_setEndHead(ne_stl_getGroup(style), head.toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

// TextFeatureMenu

TextFeatureMenu::TextFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    textFeatureTreeView = NULL;
    bBoxFMenu = NULL;
    textFontBranch = NULL;
    textAlignmentBranch = NULL;
    setLayout(&textLayout);
    
    textFeatureTreeView = new MyTreeView(parent);
    /// plain text
    connect(&plainTextWidget, SIGNAL(plainTextChanged(const QString&)), this, SLOT(changePlainText(const QString&)));
    textFeatureTreeView->addBranchWidget(&plainTextWidget, "PlainText");
    
    /// bounding box
    bBoxFMenu = new BoundingBoxFeatureMenu(this, _mw);
    connect(bBoxFMenu, SIGNAL(bBoxPositionChanged()), SIGNAL(textChanged()));
    textFeatureTreeView->addBranchWidget(bBoxFMenu, "Bounding Box");
    
    /// font
    textFontBranch = new MyGroupBox(textFeatureTreeView);
    // font color
    textFontLayout.addWidget(new MyLabel("Color:"), textFontLayout.rowCount(), 0, Qt::AlignLeft);
    fontColorPickerButton = new MyColorPickerButton(_mw);
    textFontLayout.addItem(new QSpacerItem(80, 0), textFontLayout.rowCount() - 1, 1);
    textFontLayout.addWidget(fontColorPickerButton, textFontLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(fontColorPickerButton, SIGNAL(colorChosen(const QString&, const QString&)), this, SLOT(changeFontColor(const QString&, const QString&)));
    // font size
    textFontLayout.addWidget(new MyLabel("Size:"), textFontLayout.rowCount(), 0, Qt::AlignLeft);
    textFontLayout.addWidget(&fontSizeComboBox, textFontLayout.rowCount() - 1, 1, 1, 3, Qt::AlignRight);
    connect(&fontSizeComboBox, SIGNAL(valueChanged(const QString&, const QString&)), this, SLOT(changeFontSize(const QString&, const QString&)));
    // font family
    textFontLayout.addWidget(new MyLabel("Family:"), textFontLayout.rowCount(), 0, Qt::AlignLeft);
    for (int i = 0; i < NUM_FontFamily; ++i) {
        if (!fontFamilyToStr(FontFamily(i)).empty())
            fontFamilyComboBox.addItem(fontFamilyToStr(FontFamily(i)).c_str());
    }
    textFontLayout.addItem(new QSpacerItem(80, 0), textFontLayout.rowCount() - 1, 1);
    fontFamilyComboBox.setFixedHeight(18.0);
    textFontLayout.addWidget(&fontFamilyComboBox, textFontLayout.rowCount() - 1, 2);
    connect(&fontFamilyComboBox, SIGNAL(activated(int)), this, SLOT(changeFontFamily(int)));
    // font weight
    textFontLayout.addWidget(new MyLabel("Weight:"), textFontLayout.rowCount(), 0, Qt::AlignLeft);
    for (int i = 0; i < NUM_FontWeight; ++i) {
        if (!fontWeightToStr(FontWeight(i)).empty())
            fontWeightComboBox.addItem(fontWeightToStr(FontWeight(i)).c_str());
    }
    textFontLayout.addItem(new QSpacerItem(80, 0), textFontLayout.rowCount() - 1, 1);
    fontWeightComboBox.setFixedHeight(18.0);
    textFontLayout.addWidget(&fontWeightComboBox, textFontLayout.rowCount() - 1, 2);
    connect(&fontWeightComboBox, SIGNAL(activated(int)), this, SLOT(changeFontWeight(int)));
    // font style
    textFontLayout.addWidget(new MyLabel("Style:"), textFontLayout.rowCount(), 0, Qt::AlignLeft);
    for (int i = 0; i < NUM_FontStyle; ++i) {
        if (!fontStyleToStr(FontStyle(i)).empty())
            fontStyleComboBox.addItem(fontStyleToStr(FontStyle(i)).c_str());
    }
    textFontLayout.addItem(new QSpacerItem(80, 0), textFontLayout.rowCount() - 1, 1);
    fontStyleComboBox.setFixedHeight(18.0);
    textFontLayout.addWidget(&fontStyleComboBox, textFontLayout.rowCount() - 1, 2);
    connect(&fontStyleComboBox, SIGNAL(activated(int)), this, SLOT(changeFontStyle(int)));
    textFontBranch->setLayout(&textFontLayout);
    textFeatureTreeView->addBranchWidget(textFontBranch, "Font");
    
    /// Alignment
    textAlignmentBranch = new MyGroupBox(textFeatureTreeView);
    // textanchor
    textAlignmentLayout.addWidget(new MyLabel("TextAnchor:"), textAlignmentLayout.rowCount(), 0);
    for (int i = 0; i < NUM_HTextAnchor; ++i) {
        if (!hTextAnchorToStr(HTextAnchor(i)).empty())
            textAnchorComboBox.addItem(hTextAnchorToStr(HTextAnchor(i)).c_str());
    }
    textAnchorComboBox.setFixedWidth(90.0);
    textAnchorComboBox.setFixedHeight(18.0);
    textAlignmentLayout.addWidget(&textAnchorComboBox, textAlignmentLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&textAnchorComboBox, SIGNAL(activated(int)), this, SLOT(changeTextAnchor(int)));
    // vtextanchor
    textAlignmentLayout.addWidget(new MyLabel("VTextAnchor:"), textAlignmentLayout.rowCount(), 0);
    for (int i = 0; i < NUM_VTextAnchor; ++i) {
        if (!vTextAnchorToStr(VTextAnchor(i)).empty())
            vTextAnchorComboBox.addItem(vTextAnchorToStr(VTextAnchor(i)).c_str());
    }
    vTextAnchorComboBox.setFixedWidth(90.0);
    vTextAnchorComboBox.setFixedHeight(18.0);
    textAlignmentLayout.addWidget(&vTextAnchorComboBox, textAlignmentLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&vTextAnchorComboBox, SIGNAL(activated(int)), this, SLOT(changeVTextAnchor(int)));
    textAlignmentBranch->setLayout(&textAlignmentLayout);
    textFeatureTreeView->addBranchWidget(textAlignmentBranch, "Alignment");
    textLayout.addWidget(textFeatureTreeView, textLayout.rowCount(), 0, 1, 2);
    
    // reset values
    resetValues();
}

void TextFeatureMenu::showInfo(GraphicalText* gT, NGraphicalObject* gO) {
    if (gT && gT->isSetStyle() && gT->isSetPlainText()) {
        _gText = gT;
        _gO = gO;
        
        // set plain text
        plainTextWidget.enable(gO);
        plainTextWidget.setPlainText(QString(_gText->getPlainText().c_str()));
        
        // set bounding box
        if (bBoxFMenu)
            bBoxFMenu->showInfo(_gText->getNGraphicalObject());
        
        // set font size
        if (ne_grp_isSetFontSize(ne_stl_getGroup(_gText->getStyle()))) {
            RAVector* fSize = ne_grp_getFontSize(ne_stl_getGroup(_gText->getStyle()));
            if (fSize)
                fontSizeComboBox.setCurrentValue(fSize->a(), fSize->r());
        }
        // set font family
        if (ne_grp_isSetFontFamily(ne_stl_getGroup(_gText->getStyle())))
            fontFamilyComboBox.setCurrentText(ne_grp_getFontFamily(ne_stl_getGroup(_gText->getStyle())).c_str());
        // set font weight
        if (ne_grp_isSetFontWeight(ne_stl_getGroup(_gText->getStyle())))
            fontWeightComboBox.setCurrentText(ne_grp_getFontWeight(ne_stl_getGroup(_gText->getStyle())).c_str());
        // set font style
        if (ne_grp_isSetFontStyle(ne_stl_getGroup(_gText->getStyle())))
            fontStyleComboBox.setCurrentText(ne_grp_getFontStyle(ne_stl_getGroup(_gText->getStyle())).c_str());
        // set font color
        if (ne_grp_isSetStrokeColor(ne_stl_getGroup(_gText->getStyle())))
            fontColorPickerButton->setBackgroundColor(ne_grp_getStrokeColor(ne_stl_getGroup(_gText->getStyle())).c_str());
        // set textanchor
        if (ne_grp_isSetHTextAnchor(ne_stl_getGroup(_gText->getStyle())))
            textAnchorComboBox.setCurrentText(ne_grp_getHTextAnchor(ne_stl_getGroup(_gText->getStyle())).c_str());
        // set vtextanchor
        if (ne_grp_isSetVTextAnchor(ne_stl_getGroup(_gText->getStyle())))
            vTextAnchorComboBox.setCurrentText(ne_grp_getVTextAnchor(ne_stl_getGroup(_gText->getStyle())).c_str());
    }
}

void TextFeatureMenu::resetValues() {
    plainTextWidget.resetValues();
    if (bBoxFMenu)
        bBoxFMenu->resetValues();
    fontSizeComboBox.resetValues();
    fontFamilyComboBox.setCurrentIndex(0);
    fontWeightComboBox.setCurrentIndex(0);
    fontStyleComboBox.setCurrentIndex(0);
    fontColorPickerButton->setBackgroundColor("White");
    textAnchorComboBox.setCurrentIndex(0);
    vTextAnchorComboBox.setCurrentIndex(0);
    
    _gText = NULL;
    _gO = NULL;
}


void TextFeatureMenu::changePlainText(const QString& plainText) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->isSetNGraphicalObject()) {
        if (!plainText.isEmpty() && plainText != "N/A")
            ne_gtxt_setPlainText((NText*)(_gText->getNGraphicalObject()), plainText.toStdString());
        else
            ne_gtxt_unSetPlainText((NText*)(_gText->getNGraphicalObject()));
            
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit textChanged();
    }
}

void TextFeatureMenu::changeFontSize(const QString& abs, const QString& rel) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->getGraphicalItems().size()) {
        RAVector* fontSize = new RAVector(abs.toInt(), rel.toInt());
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());
        
        if (!ne_grp_setFontSize(ne_stl_getGroup(style), fontSize)) {
            _gText->GraphicalObjectBase::setStyle(style);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
        
        delete fontSize;
    }
}

void TextFeatureMenu::changeFontFamily(int index) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->getGraphicalItems().size() && !fontFamilyComboBox.itemText(index).isEmpty()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());
        
        if (!ne_grp_setFontFamily(ne_stl_getGroup(style), fontFamilyComboBox.itemText(index).toStdString())) {
            _gText->GraphicalObjectBase::setStyle(style);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontWeight(int index) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->getGraphicalItems().size() && !fontWeightComboBox.itemText(index).isEmpty()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());

        if (!ne_grp_setFontWeight(ne_stl_getGroup(style), fontWeightComboBox.itemText(index).toStdString())) {
            _gText->GraphicalObjectBase::setStyle(style);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontStyle(int index) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->getGraphicalItems().size() && !fontStyleComboBox.itemText(index).isEmpty()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());
        
        if (!ne_grp_setFontStyle(ne_stl_getGroup(style), fontStyleComboBox.itemText(index).toStdString())) {
            _gText->GraphicalObjectBase::setStyle(style);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontColor(const QString& color, const QString& value) {
    if (!ne_ven_getColor(_mw->getSBMLDocument()->getVeneer(), -1, color.toStdString())) {
        VColorDefinition* c = ne_ven_addNewColor(_mw->getSBMLDocument()->getVeneer(), color.toStdString(), value.toStdString());
        
        // add the color to list of colors
        if (c) {
            _mw->getSBMLDocument()->setRenderModified(true);
            // create a new graphical color
            GraphicalColor* _gColor = new GraphicalColor();
            // get info from veneer color
            getInfoFromColor(_gColor, c);
            // store the graphical color
            _mw->addGColor(_gColor);
        }
    }
    
    VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());
    
    if (!ne_grp_setStrokeColor(ne_stl_getGroup(style), color.toStdString())) {
        _gText->GraphicalObjectBase::setStyle(style);
        _mw->getSBMLDocument()->setRenderModified(true);
        emit textChanged();
    }
}

void TextFeatureMenu::changeTextAnchor(int index) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->getGraphicalItems().size() && !textAnchorComboBox.itemText(index).isEmpty()) {
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());
        
        if (!ne_grp_setHTextAnchor(ne_stl_getGroup(style), textAnchorComboBox.itemText(index).toStdString())) {
            _gText->GraphicalObjectBase::setStyle(style);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeVTextAnchor(int index) {
    if (_mw && _mw->isSetSBMLDocument() && _gText && _gText->getGraphicalItems().size() && !vTextAnchorComboBox.itemText(index).isEmpty()) {
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gText->getNGraphicalObject(), _gText->getStyle());
        
        if (!ne_grp_setVTextAnchor(ne_stl_getGroup(style), vTextAnchorComboBox.itemText(index).toStdString())) {
            _gText->GraphicalObjectBase::setStyle(style);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

// GeometricShapeFeatureMenuElement

GeometricShapeFeatureMenuElement::GeometricShapeFeatureMenuElement(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    gShapeTreeView = NULL;
    strokeFMenu = NULL;
    fillFMenu = NULL;
    setLayout(&gShapeLayout);
}

void GeometricShapeFeatureMenuElement::collapseTree() {
    if (gShapeTreeView)
        gShapeTreeView->collapseAll();
}

// RectangleGeometricShapeFeatureMenu

RectangleGeometricShapeFeatureMenu::RectangleGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw, const bool& isLineEnding) : GeometricShapeFeatureMenuElement(parent, mw) {
    rectanglePositionBranch = NULL;
    rectangleDimensionsBranch = NULL;
    rectangleCornerCurvaturesBranch = NULL;
    
    gShapeTreeView = new MyTreeView(parent);
    /// features
    // stroke
    if (!isLineEnding) {
        strokeFMenu = new StrokeFeatureMenu(gShapeTreeView, _mw);
        connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
        gShapeTreeView->addBranchWidget(strokeFMenu, "Stroke");
    }
    // fill
    fillFMenu = new FillFeatureMenu(gShapeTreeView, _mw);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
    gShapeTreeView->addBranchWidget(fillFMenu, "Fill");

    /// position
    rectanglePositionBranch = new MyGroupBox(gShapeTreeView);
    // x label
    rectanglePositionLayout.addWidget(new MyLabel("X:"), rectanglePositionLayout.rowCount(), 0, Qt::AlignLeft);
    // x
    rectanglePositionLayout.addWidget(&positionXRelAbsSpinBox, rectanglePositionLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&positionXRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePositionX(const double&, const double&)));
    // y label
    rectanglePositionLayout.addWidget(new MyLabel("Y:"), rectanglePositionLayout.rowCount(), 0, Qt::AlignLeft);
    // y
    rectanglePositionLayout.addWidget(&positionYRelAbsSpinBox, rectanglePositionLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&positionYRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePositionY(const double&, const double&)));
    rectanglePositionBranch->setLayout(&rectanglePositionLayout);
    gShapeTreeView->addBranchWidget(rectanglePositionBranch, "Position");
    
    /// dimensions
    rectangleDimensionsBranch = new MyGroupBox(gShapeTreeView);
    // width label
    rectangleDimensionsLayout.addWidget(new MyLabel("Width:"), rectangleDimensionsLayout.rowCount(), 0, Qt::AlignLeft);
    // width
    rectangleDimensionsLayout.addWidget(&dimensionWidthRelAbsSpinBox, rectangleDimensionsLayout.rowCount() - 1, 1, 1, 3, Qt::AlignRight);
    connect(&dimensionWidthRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensionWidth(const double&, const double&)));
    // height label
    rectangleDimensionsLayout.addWidget(new MyLabel("Height:"), rectangleDimensionsLayout.rowCount(), 0, Qt::AlignLeft);
    // height
    rectangleDimensionsLayout.addWidget(&dimensionHeightRelAbsSpinBox, rectangleDimensionsLayout.rowCount() - 1, 1, 1, 3, Qt::AlignRight);
    connect(&dimensionHeightRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensionHeight(const double&, const double&)));
    // ratio label
    rectangleDimensionsLayout.addWidget(new MyLabel("Ratio:"), rectangleDimensionsLayout.rowCount(), 0, Qt::AlignLeft);
    // ratio
    rectangleDimensionsLayout.addItem(new QSpacerItem(90, 0), rectangleDimensionsLayout.rowCount() - 1, 1);
    rectangleDimensionsLayout.addWidget(&dimensionRatioComboBox, rectangleDimensionsLayout.rowCount() - 1, 2);
    connect(&dimensionRatioComboBox, SIGNAL(ratioChosen(const double&)), this, SLOT(changeDimensionRatio(const double&)));
    rectangleDimensionsBranch->setLayout(&rectangleDimensionsLayout);
    gShapeTreeView->addBranchWidget(rectangleDimensionsBranch, "Dimensions");
    
    /// corner curvatures
    rectangleCornerCurvaturesBranch = new MyGroupBox(gShapeTreeView);
    // rx label
    rectangleCornerCurvaturesLayout.addWidget(new MyLabel("RX:"), rectangleCornerCurvaturesLayout.rowCount(), 0, Qt::AlignLeft);
    // rx
    rectangleCornerCurvaturesLayout.addWidget(&cornerCurvatureRXRelAbsSpinBox, rectangleCornerCurvaturesLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&cornerCurvatureRXRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeCornerCurvatureRX(const double&, const double&)));
    // ry label
    rectangleCornerCurvaturesLayout.addWidget(new MyLabel("RY:"), rectangleCornerCurvaturesLayout.rowCount(), 0, Qt::AlignLeft);
    // ry
    rectangleCornerCurvaturesLayout.addWidget(&cornerCurvatureRYRelAbsSpinBox, rectangleCornerCurvaturesLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&cornerCurvatureRYRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeCornerCurvatureRY(const double&, const double&)));
    rectangleCornerCurvaturesBranch->setLayout(&rectangleCornerCurvaturesLayout);
    gShapeTreeView->addBranchWidget(rectangleCornerCurvaturesBranch, "Corner Curvatures");
    gShapeLayout.addWidget(gShapeTreeView, gShapeLayout.rowCount(), 0, 1, 2);
    
    // reset values
    resetValues();
}

void RectangleGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex)) == 3) {
        _style = style;
        _gO = gO;
        getInfoFromRectangleShape((VRectangle*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex));
    }
}

void RectangleGeometricShapeFeatureMenu::showInfo(VLineEnding* lE, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), _gSIndex)) == 3) {
        _lE = lE;
        getInfoFromRectangleShape((VRectangle*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex));
    }
}

void RectangleGeometricShapeFeatureMenu::resetValues() {
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    positionXRelAbsSpinBox.resetValues();
    positionYRelAbsSpinBox.resetValues();
    dimensionWidthRelAbsSpinBox.resetValues();
    dimensionHeightRelAbsSpinBox.resetValues();
    dimensionRatioComboBox.resetValues();
    cornerCurvatureRXRelAbsSpinBox.resetValues();
    cornerCurvatureRYRelAbsSpinBox.resetValues();
    
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gSIndex = 0;
}

void RectangleGeometricShapeFeatureMenu::getInfoFromRectangleShape(VRectangle* rectangle) {
    if (rectangle) {
        if (strokeFMenu)
            strokeFMenu->showInfo(_gO, _style, _gSIndex);
        if (fillFMenu)
            fillFMenu->showInfo(_gO, _style, _gSIndex);
        
        RAVector* point = NULL;
        QString sPoint;
        // set x
        if (ne_rec_isSetPositionX(rectangle)) {
            point = ne_rec_getPositionX(rectangle);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    positionXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_go_getBoundingBox(_gO)) + ne_bb_getWidth(ne_go_getBoundingBox(_gO))));
                else if (ne_le_isSetBoundingBox(_lE))
                    positionXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_le_getBoundingBox(_lE)) + ne_bb_getWidth(ne_le_getBoundingBox(_lE))));
                positionXRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set y
        if (ne_rec_isSetPositionY(rectangle)) {
            point = ne_rec_getPositionY(rectangle);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    positionYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_go_getBoundingBox(_gO)) + ne_bb_getHeight(ne_go_getBoundingBox(_gO))));
                else if (ne_le_isSetBoundingBox(_lE))
                    positionYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_le_getBoundingBox(_lE)) + ne_bb_getHeight(ne_le_getBoundingBox(_lE))));
                positionYRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set width
        if (ne_rec_isSetDimensionWidth(rectangle)) {
            point = ne_rec_getDimensionWidth(rectangle);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    dimensionWidthRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    dimensionWidthRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
                dimensionWidthRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set height
        if (ne_rec_isSetDimensionHeight(rectangle)) {
            point = ne_rec_getDimensionHeight(rectangle);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    dimensionHeightRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    dimensionHeightRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
                dimensionHeightRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set ratio
        if (ne_rec_isSetRatio(rectangle))
            dimensionRatioComboBox.setRatio(ne_rec_getRatio(rectangle));
        // set rx
        if (ne_rec_isSetCornerCurvatureRX(rectangle)) {
            point = ne_rec_getCornerCurvatureRX(rectangle);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    cornerCurvatureRXRelAbsSpinBox.setRange(0.0, ne_bb_getWidth(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    cornerCurvatureRXRelAbsSpinBox.setRange(0.0, ne_bb_getWidth(ne_le_getBoundingBox(_lE)));
                cornerCurvatureRXRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set ry
        if (ne_rec_isSetCornerCurvatureRY(rectangle)) {
            point = ne_rec_getCornerCurvatureRY(rectangle);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    cornerCurvatureRYRelAbsSpinBox.setRange(0.0, ne_bb_getHeight(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    cornerCurvatureRYRelAbsSpinBox.setRange(0.0, ne_bb_getHeight(ne_le_getBoundingBox(_lE)));
                cornerCurvatureRYRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changePositionX(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setPositionX(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setPositionX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pX;
    }
}

void RectangleGeometricShapeFeatureMenu::changePositionY(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pY = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setPositionY(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setPositionY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pY;
    }
}

void RectangleGeometricShapeFeatureMenu::changeDimensionWidth(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pWidth = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setDimensionWidth(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setDimensionWidth(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pWidth;
    }
}

void RectangleGeometricShapeFeatureMenu::changeDimensionHeight(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pHeight = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setDimensionHeight(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setDimensionHeight(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pHeight;
    }
}

void RectangleGeometricShapeFeatureMenu::changeCornerCurvatureRX(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pRX = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setCornerCurvatureRX(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setCornerCurvatureRX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pRX;
    }
}

void RectangleGeometricShapeFeatureMenu::changeCornerCurvatureRY(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pRY = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setCornerCurvatureRY(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setCornerCurvatureRY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pRY;
    }
}

void RectangleGeometricShapeFeatureMenu::changeDimensionRatio(const double& ratio) {
    if (_mw && _mw->isSetSBMLDocument()) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setRatio(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), ratio)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setRatio(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), ratio)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// ImageGeometricShapeFeatureMenu

ImageGeometricShapeFeatureMenu::ImageGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw) : GeometricShapeFeatureMenuElement(parent, mw) {
    imagePositionBranch = NULL;
    imageDimensionsBranch = NULL;
    imagePathBranch = NULL;
    
    gShapeTreeView = new MyTreeView(parent);
    /// position
    imagePositionBranch = new MyGroupBox(gShapeTreeView);
    // x label
    imagePositionLayout.addWidget(new MyLabel("X:"), imagePositionLayout.rowCount(), 1, Qt::AlignLeft);
    // x
    imagePositionLayout.addWidget(&positionXRelAbsSpinBox, imagePositionLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionXRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePositionX(const double&, const double&)));
    // y label
    imagePositionLayout.addWidget(new MyLabel("Y:"), imagePositionLayout.rowCount(), 1, Qt::AlignLeft);
    // y
    imagePositionLayout.addWidget(&positionYRelAbsSpinBox, imagePositionLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionYRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePositionY(const double&, const double&)));
    imagePositionBranch->setLayout(&imagePositionLayout);
    gShapeTreeView->addBranchWidget(imagePositionBranch, "Position");
    
    /// dimensions
    imageDimensionsBranch = new MyGroupBox(gShapeTreeView);
    // width label
    imageDimensionsLayout.addWidget(new MyLabel("Width:"), imageDimensionsLayout.rowCount(), 1, Qt::AlignLeft);
    // width
    imageDimensionsLayout.addWidget(&dimensionWidthRelAbsSpinBox, imageDimensionsLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionWidthRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensionWidth(const double&, const double&)));
    // height label
    imageDimensionsLayout.addWidget(new MyLabel("Height:"), imageDimensionsLayout.rowCount(), 1, Qt::AlignLeft);
    // height
    imageDimensionsLayout.addWidget(&dimensionHeightRelAbsSpinBox, imageDimensionsLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionHeightRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensionHeight(const double&, const double&)));
    imageDimensionsBranch->setLayout(&imageDimensionsLayout);
    gShapeTreeView->addBranchWidget(imageDimensionsBranch, "Dimensions");
    
    /// path
    imagePathBranch = new MyGroupBox(gShapeTreeView);
    // href label
    imagePathLayout.addWidget(new MyLabel("href:"), imagePathLayout.rowCount(), 1, Qt::AlignLeft);
    // href
    imagePathLayout.addWidget(&hrefButton, imagePathLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&hrefButton, SIGNAL(clicked()), this, SLOT(changeHref()));
    imagePathBranch->setLayout(&imagePathLayout);
    gShapeTreeView->addBranchWidget(imagePathBranch, "Path");
    gShapeLayout.addWidget(gShapeTreeView, gShapeLayout.rowCount(), 0, 1, 2);

    // reset values
    resetValues();
}

void ImageGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex)) == 0) {
        _style = style;
        _gO = gO;
        getInfoFromImageShape((VImage*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex));
    }
}

void ImageGeometricShapeFeatureMenu::showInfo(VLineEnding* lE, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), _gSIndex)) == 0) {
        _lE = lE;
        getInfoFromImageShape((VImage*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex));
    }
}

void ImageGeometricShapeFeatureMenu::resetValues() {
    positionXRelAbsSpinBox.resetValues();
    positionYRelAbsSpinBox.resetValues();
    dimensionWidthRelAbsSpinBox.resetValues();
    dimensionHeightRelAbsSpinBox.resetValues();
    hrefButton.setText("N/A");
    href.clear();
    
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gSIndex = 0;
}

void ImageGeometricShapeFeatureMenu::getInfoFromImageShape(VImage* image) {
    if (image) {
        RAVector* point = NULL;
        QString sPoint;
        // set x
        if (ne_img_isSetPositionX(image)) {
            point = ne_img_getPositionX(image);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    positionXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_go_getBoundingBox(_gO)) + ne_bb_getWidth(ne_go_getBoundingBox(_gO))));
                else if (_lE && _lE->isSetBox())
                    positionXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_le_getBoundingBox(_lE)) + ne_bb_getWidth(ne_le_getBoundingBox(_lE))));
                positionXRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set y
        if (ne_img_isSetPositionY(image)) {
            point = ne_img_getPositionY(image);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    positionYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_go_getBoundingBox(_gO)) + ne_bb_getHeight(ne_go_getBoundingBox(_gO))));
                else if (_lE && _lE->isSetBox())
                    positionYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_le_getBoundingBox(_lE)) + ne_bb_getHeight(ne_le_getBoundingBox(_lE))));
                positionYRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set width
        if (ne_img_isSetDimensionWidth(image)) {
            point = ne_img_getDimensionWidth(image);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    dimensionWidthRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    dimensionWidthRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
                dimensionWidthRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set height
        if (ne_img_isSetDimensionHeight(image)) {
            point = ne_img_getDimensionHeight(image);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    dimensionHeightRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    dimensionHeightRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
                dimensionHeightRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set href
        if (ne_img_isSetHref(image)) {
            href = QString(ne_img_getHref(image).c_str());
            QString buttonText = QFileInfo(href).fileName();
            if (buttonText.size() > 16) {
                buttonText = buttonText.split(".").at(0);
                buttonText.remove(4, buttonText.size() - 8);
                buttonText.insert(4, "...");
                buttonText += "." + QFileInfo(href).suffix();
            }
            hrefButton.setText(buttonText);
        }
    }
}

void ImageGeometricShapeFeatureMenu::changePositionX(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setPositionX(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setPositionX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pX;
    }
}

void ImageGeometricShapeFeatureMenu::changePositionY(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pY = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setPositionY(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setPositionY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pY;
    }
}

void ImageGeometricShapeFeatureMenu::changeDimensionWidth(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pWidth = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setDimensionWidth(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setDimensionWidth(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pWidth;
    }
}

void ImageGeometricShapeFeatureMenu::changeDimensionHeight(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pHeight = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setDimensionHeight(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setDimensionHeight(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pHeight;
    }
}

void ImageGeometricShapeFeatureMenu::changeHref() {
    QString href = QFileDialog::getOpenFileName(this, "Select an image file", ".", tr("Image Files (*.png *.jpg *.jpeg)"));
    
    if (_mw->isSetSBMLDocument() && !href.isEmpty()) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setHref(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), href.toStdString())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setHref(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), href.toStdString())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// EllipseGeometricShapeFeatureMenu

EllipseGeometricShapeFeatureMenu::EllipseGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw, const bool& isLineEnding) : GeometricShapeFeatureMenuElement(parent, mw) {
    ellipsePositionBranch = NULL;
    ellipseDimensionsBranch = NULL;
    
    gShapeTreeView = new MyTreeView(this);
    /// features
    // stroke
    if (!isLineEnding) {
        strokeFMenu = new StrokeFeatureMenu(gShapeTreeView, _mw);
        connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
        gShapeTreeView->addBranchWidget(strokeFMenu, "Stroke");
    }
    // fill
    fillFMenu = new FillFeatureMenu(gShapeTreeView, _mw);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
    gShapeTreeView->addBranchWidget(fillFMenu, "Fill");
    
    /// position
    ellipsePositionBranch = new MyGroupBox(gShapeTreeView);
    // cx label
    ellipsePositionLayout.addWidget(new MyLabel("CX:"), ellipsePositionLayout.rowCount(), 0, Qt::AlignLeft);
    // cx
    ellipsePositionLayout.addWidget(&positionCXRelAbsSpinBox, ellipsePositionLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&positionCXRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePositionCX(const double&, const double&)));
    // cy label
    ellipsePositionLayout.addWidget(new MyLabel("CY:"), ellipsePositionLayout.rowCount(), 0, Qt::AlignLeft);
    // cy
    ellipsePositionLayout.addWidget(&positionCYRelAbsSpinBox, ellipsePositionLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&positionCYRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changePositionCY(const double&, const double&)));
    ellipsePositionBranch->setLayout(&ellipsePositionLayout);
    gShapeTreeView->addBranchWidget(ellipsePositionBranch, "Position");
    
    /// dimensions
    ellipseDimensionsBranch = new MyGroupBox(gShapeTreeView);
    // rx label
    ellipseDimensionsLayout.addWidget(new MyLabel("RX:"), ellipseDimensionsLayout.rowCount(), 0, Qt::AlignLeft);
    // rx
    ellipseDimensionsLayout.addWidget(&dimensionRXRelAbsSpinBox, ellipseDimensionsLayout.rowCount() - 1, 1, 1, 3, Qt::AlignRight);
    connect(&dimensionRXRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensionRX(const double&, const double&)));
    // ry label
    ellipseDimensionsLayout.addWidget(new MyLabel("RY:"), ellipseDimensionsLayout.rowCount(), 0, Qt::AlignLeft);
    // ry
    ellipseDimensionsLayout.addWidget(&dimensionRYRelAbsSpinBox, ellipseDimensionsLayout.rowCount() - 1, 1, 1, 3, Qt::AlignRight);
    connect(&dimensionRYRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeDimensionRY(const double&, const double&)));
    // ratio label
    ellipseDimensionsLayout.addWidget(new MyLabel("Ratio:"), ellipseDimensionsLayout.rowCount(), 0, Qt::AlignLeft);
    // ratio
    ellipseDimensionsLayout.addItem(new QSpacerItem(100, 0), ellipseDimensionsLayout.rowCount() - 1, 1);
    ellipseDimensionsLayout.addWidget(&dimensionRatioComboBox, ellipseDimensionsLayout.rowCount() - 1, 2);
    connect(&dimensionRatioComboBox, SIGNAL(ratioChosen(const double&)), this, SLOT(changeDimensionRatio(const double&)));
    ellipseDimensionsBranch->setLayout(&ellipseDimensionsLayout);
    gShapeTreeView->addBranchWidget(ellipseDimensionsBranch, "Dimensions");
    gShapeLayout.addWidget(gShapeTreeView, gShapeLayout.rowCount(), 0, 1, 2);
    
    // reset values
    resetValues();
}

void EllipseGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex)) == 4) {
        _style = style;
        _gO = gO;
        getInfoFromEllipseShape((VEllipse*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex));
    }
}

void EllipseGeometricShapeFeatureMenu::showInfo(VLineEnding* lE, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), _gSIndex)) == 4) {
        _lE = lE;
        getInfoFromEllipseShape((VEllipse*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex));
    }
}

void EllipseGeometricShapeFeatureMenu::resetValues() {
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    positionCXRelAbsSpinBox.resetValues();
    positionCYRelAbsSpinBox.resetValues();
    dimensionRXRelAbsSpinBox.resetValues();
    dimensionRYRelAbsSpinBox.resetValues();
    dimensionRatioComboBox.resetValues();
    
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gSIndex = 0;
}

void EllipseGeometricShapeFeatureMenu::getInfoFromEllipseShape(VEllipse* ellipse) {
    if (ellipse) {
        if (strokeFMenu)
            strokeFMenu->showInfo(_gO, _style, _gSIndex);
        if (fillFMenu)
            fillFMenu->showInfo(_gO, _style, _gSIndex);
        
        RAVector* point = NULL;
        QString sPoint;
        // set x
        if (ne_elp_isSetPositionCX(ellipse)) {
            point = ne_elp_getPositionCX(ellipse);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    positionCXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_go_getBoundingBox(_gO)) + ne_bb_getWidth(ne_go_getBoundingBox(_gO))));
                else if (ne_le_isSetBoundingBox(_lE))
                    positionCXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_le_getBoundingBox(_lE)) + ne_bb_getWidth(ne_le_getBoundingBox(_lE))));
                positionCXRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set cy
        if (ne_elp_isSetPositionCY(ellipse)) {
            point = ne_elp_getPositionCY(ellipse);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    positionCYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_go_getBoundingBox(_gO)) + ne_bb_getHeight(ne_go_getBoundingBox(_gO))));
                else if (ne_le_isSetBoundingBox(_lE))
                    positionCYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_le_getBoundingBox(_lE)) + ne_bb_getHeight(ne_le_getBoundingBox(_lE))));
                positionCYRelAbsSpinBox.setCurrentValue(point->a(), point->r());
            }
        }
        // set rx
        if (ne_elp_isSetDimensionRX(ellipse)) {
            point = ne_elp_getDimensionRX(ellipse);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    dimensionRXRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    dimensionRXRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
                dimensionRXRelAbsSpinBox.setCurrentValue(point->a(), point->r());
                
                if (!ne_elp_isSetDimensionRY(ellipse)) {
                    if (ne_go_isSetBoundingBox(_gO))
                        dimensionRYRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
                    else if (ne_le_isSetBoundingBox(_lE))
                        dimensionRYRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
                    dimensionRYRelAbsSpinBox.setCurrentValue(point->a(), point->r());
                }
            }
        }
        // set ry
        if (ne_elp_isSetDimensionRY(ellipse)) {
            point = ne_elp_getDimensionRY(ellipse);
            if (point) {
                if (ne_go_isSetBoundingBox(_gO))
                    dimensionRYRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
                else if (ne_le_isSetBoundingBox(_lE))
                    dimensionRYRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
                dimensionRYRelAbsSpinBox.setCurrentValue(point->a(), point->r());
                
                if (!ne_elp_isSetDimensionRX(ellipse)) {
                    if (ne_go_isSetBoundingBox(_gO))
                        dimensionRXRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
                    else if (ne_le_isSetBoundingBox(_lE))
                        dimensionRXRelAbsSpinBox.setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
                    dimensionRXRelAbsSpinBox.setCurrentValue(point->a(), point->r());
                }
            }
        }
        // set ratio
        if (ne_elp_isSetRatio(ellipse))
            dimensionRatioComboBox.setRatio(ne_elp_getRatio(ellipse));
    }
}

void EllipseGeometricShapeFeatureMenu::changePositionCX(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pCX = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setPositionCX(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pCX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setPositionCX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pCX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pCX;
    }
}

void EllipseGeometricShapeFeatureMenu::changePositionCY(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pCY = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setPositionCY(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pCY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setPositionCY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pCY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pCY;
    }
}

void EllipseGeometricShapeFeatureMenu::changeDimensionRX(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pRX = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setDimensionRX(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setDimensionRX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pRX;
    }
}

void EllipseGeometricShapeFeatureMenu::changeDimensionRY(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pRY = new RAVector(abs, rel);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setDimensionRY(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setDimensionRY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pRY;
    }
}

void EllipseGeometricShapeFeatureMenu::changeDimensionRatio(const double& ratio) {
    if (_mw && _mw->isSetSBMLDocument() && _gO) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setRatio(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), ratio)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setRatio(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), ratio)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// PolygonGeometricShapeFeatureMenu

PolygonGeometricShapeFeatureMenu::PolygonGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw, const bool& isRCurve, const bool& isLineEnding) : GeometricShapeFeatureMenuElement(parent, mw) {
    gShapeVerticesBranch = NULL;
    gShapeType = GRP_ELT_SHPE_PLG;
        
    /// features
    gShapeTreeView = new MyTreeView(parent);
    // stroke
    if (!isLineEnding) {
        strokeFMenu = new StrokeFeatureMenu(gShapeTreeView, _mw);
        connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
        gShapeTreeView->addBranchWidget(strokeFMenu, "Stroke");
    }
    // fill
    if (!isRCurve) {
        fillFMenu = new FillFeatureMenu(gShapeTreeView, _mw);
        connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
        gShapeTreeView->addBranchWidget(fillFMenu, "Fill");
    }
    
    // vertices
    gShapeVerticesBranch = new MyGroupBox(gShapeTreeView);
    gShapeVerticesLayout.addWidget(&addRemoveVertexButtons, gShapeVerticesLayout.rowCount(), 2);
    connect(&addRemoveVertexButtons, SIGNAL(addItemFeaturesChosen(const unsigned int&, const double&, const double&, const double&, const double&)), this, SLOT(addPoint(const unsigned int&, const double&, const double&, const double&, const double&)));
    connect(&addRemoveVertexButtons, SIGNAL(addItemFeaturesChosen(const unsigned int&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&)), this, SLOT(addCubicBezier(const unsigned int&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&, const double&)));
    connect(&addRemoveVertexButtons, SIGNAL(removeItemFeaturesChosen(const unsigned int&)), this, SLOT(removePoint(const unsigned int&)));
    gShapeVerticesBranch->setLayout(&gShapeVerticesLayout);
    gShapeTreeView->addBranchWidget(gShapeVerticesBranch, "Vertices");
    gShapeLayout.addWidget(gShapeTreeView, gShapeLayout.rowCount(), 0, 1, 2);
    
    // reset values
    resetValues();
}

void PolygonGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex)) == 5 || ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex)) == 1) {
        _style = style;
        _gO = gO;
        getInfoFromGShape(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex));
    }
}

void PolygonGeometricShapeFeatureMenu::showInfo(VLineEnding* lE, const unsigned int& gSIndex) {
    _gSIndex = gSIndex;
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), _gSIndex)) == 5 || ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), _gSIndex)) == 1) {
        _lE = lE;
        getInfoFromGShape(ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex));
    }
}

void PolygonGeometricShapeFeatureMenu::resetValues() {
    if (strokeFMenu)
        strokeFMenu->resetValues();
    if (fillFMenu)
        fillFMenu->resetValues();
    for (constPointFeatureMenuIt pFMIt = pointFeatureMenusBegin(); pFMIt != pointFeatureMenusEnd(); ++ pFMIt) {
        (*pFMIt)->resetValues();
        gShapeLayout.removeWidget(*pFMIt);
        (*pFMIt)->deleteLater();
    }
    _pointFeatureMenus.clear();
    if (gShapeTreeView)
        gShapeTreeView->removeBranches("Vertices", 1);
    addRemoveVertexButtons.resetValues();
    
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gSIndex = 0;
}

void PolygonGeometricShapeFeatureMenu::collapseTree() {
    GeometricShapeFeatureMenuElement::collapseTree();
}

void PolygonGeometricShapeFeatureMenu::getInfoFromGShape(VTransformation2D* t2d) {
    if (t2d && ne_gs_getShape(t2d) == 5) {
        if (strokeFMenu)
            strokeFMenu->showInfo(_gO, _style, _gSIndex);
        if (fillFMenu)
            fillFMenu->showInfo(_gO, _style, _gSIndex);
        RenderPointFeatureMenu* pFeatureMenu = NULL;
        for (int i = 0; i < ne_plg_getNumVertices(t2d); ++i) {
            if (ne_vrx_isRenderCubicBezier(ne_plg_getVertex(t2d, i)))
                pFeatureMenu = new RenderCubicBezierFeatureMenu(this, _mw);
            else
                pFeatureMenu = new RenderPointFeatureMenu(this, _mw);
            
            if (_gO && _style) {
                connect(pFeatureMenu, SIGNAL(styleChanged(VLocalStyle*)), SIGNAL(styleChanged(VLocalStyle*)));
                pFeatureMenu->showInfo(_gO, _style, _gSIndex, i);
            }
            else if (_lE) {
                connect(pFeatureMenu, SIGNAL(lEndingChanged(VLineEnding*)), SIGNAL(lEndingChanged(VLineEnding*)));
                pFeatureMenu->showInfo(_lE, _gSIndex, i);
            }
            _pointFeatureMenus.push_back(pFeatureMenu);
            gShapeTreeView->addBranchWidget(pFeatureMenu, "Vertex " + QString::number(i + 1), "Vertices");
        }
        
        if (_gO)
            addRemoveVertexButtons.enableAddButton(_mw, ne_go_getBoundingBox(_gO), _pointFeatureMenus.size());
        else if (_lE)
            addRemoveVertexButtons.enableAddButton(_mw, ne_le_getBoundingBox(_lE), _pointFeatureMenus.size());
        
        if (_pointFeatureMenus.size() > 3)
            addRemoveVertexButtons.enableRemoveButton(_pointFeatureMenus.size());
    }
}

void PolygonGeometricShapeFeatureMenu::addPoint(const unsigned int& itemIndex, const double& itemPointXAbsoluteValue, const double& itemPointXRelativeValue, const double& itemPointYAbsoluteValue, const double& itemPointYRelativeValue) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RPoint* rP = new RPoint(RAVector(itemPointXAbsoluteValue, itemPointXRelativeValue), RAVector(itemPointYAbsoluteValue, itemPointYRelativeValue));
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            if (gShapeType == 5)
                ne_plg_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), rP, NULL, NULL, itemIndex);
            else if (gShapeType == 1)
                ne_rc_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), rP, NULL, NULL, itemIndex);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            if (gShapeType == 5)
                ne_plg_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), rP, NULL, NULL, itemIndex);
            else if (gShapeType == 1)
                ne_rc_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), rP, NULL, NULL, itemIndex);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

void PolygonGeometricShapeFeatureMenu::addCubicBezier(const unsigned int& itemIndex, const double& itemCubicBezierXAbsoluteValue, const double& itemCubicBezierXRelativeValue, const double& itemCubicBezierYAbsoluteValue, const double& itemCubicBezierYRelativeValue, const double& itemCubicBezierC1XAbsoluteValue, const double& itemCubicBezierC1XRelativeValue, const double& itemCubicBezierC1YAbsoluteValue, const double& itemCubicBezierC1YRelativeValue, const double& itemCubicBezierC2XAbsoluteValue, const double& itemCubicBezierC2XRelativeValue, const double& itemCubicBezierC2YAbsoluteValue, const double& itemCubicBezierC2YRelativeValue) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RPoint* rP = new RPoint(RAVector(itemCubicBezierXAbsoluteValue, itemCubicBezierXRelativeValue), RAVector(itemCubicBezierYAbsoluteValue, itemCubicBezierYRelativeValue));
        RPoint* bP1 = new RPoint(RAVector(itemCubicBezierC1XAbsoluteValue, itemCubicBezierC1XRelativeValue), RAVector(itemCubicBezierC1YAbsoluteValue, itemCubicBezierC1YRelativeValue));
        RPoint* bP2 = new RPoint(RAVector(itemCubicBezierC2XAbsoluteValue, itemCubicBezierC2XRelativeValue), RAVector(itemCubicBezierC2YAbsoluteValue, itemCubicBezierC2YRelativeValue));
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            if (gShapeType == 5)
                ne_plg_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), rP, bP1, bP2, itemIndex);
            else if (gShapeType == 1)
                ne_rc_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), rP, bP1, bP2, itemIndex);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            if (gShapeType == 5)
                ne_plg_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), rP, bP1, bP2, itemIndex);
            else if (gShapeType == 1)
                ne_rc_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), rP, bP1, bP2, itemIndex);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

void PolygonGeometricShapeFeatureMenu::removePoint(const unsigned int& itemIndex) {
    if (_mw && _mw->isSetSBMLDocument()) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            if (gShapeType == 5)
                ne_plg_removeVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), itemIndex);
            else if (gShapeType == 1)
                ne_rc_removeVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), itemIndex);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            if (gShapeType == 5)
                ne_plg_removeVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), itemIndex);
            else if (gShapeType == 1)
                ne_rc_removeVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), itemIndex);
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

// RenderCurveGeometricShapeFeatureMenu

RenderCurveGeometricShapeFeatureMenu::RenderCurveGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw, const bool& isLineEnding) : PolygonGeometricShapeFeatureMenu(parent, mw, true, isLineEnding) {
    gShapeType = GRP_ELT_SHPE_CRV;
    headFMenu = NULL;
    
    // head
    headFMenu = new HeadFeatureMenu(gShapeTreeView, _mw);
    connect(headFMenu, SIGNAL(styleChanged(VLocalStyle*)), this, SIGNAL(styleChanged(VLocalStyle*)));
    connect(_mw, SIGNAL(numberOfGLEndingChanged()), headFMenu, SLOT(updateValues()));
    gShapeTreeView->addBranchWidget(headFMenu, "Heads");
    
    // reset values
    resetValues();
};

void RenderCurveGeometricShapeFeatureMenu::resetValues() {
    PolygonGeometricShapeFeatureMenu::resetValues();
    if (headFMenu)
        headFMenu->resetValues();
}

void RenderCurveGeometricShapeFeatureMenu::getInfoFromGShape(VTransformation2D* t2d) {
    if (t2d && ne_gs_getShape(t2d) == 1) {
        if (strokeFMenu)
            strokeFMenu->showInfo(_gO, _style, _gSIndex);
        if (headFMenu)
            headFMenu->showInfo(_gO, _style, _gSIndex);
        
        RenderPointFeatureMenu* pFeatureMenu = NULL;
        for (int i = 0; i < ne_rc_getNumVertices(t2d); ++i) {
            if (ne_vrx_isRenderCubicBezier(ne_plg_getVertex(t2d, i)))
                pFeatureMenu = new RenderCubicBezierFeatureMenu(this, _mw);
            else
                pFeatureMenu = new RenderPointFeatureMenu(this, _mw);
            
            if (_gO && _style) {
                connect(pFeatureMenu, SIGNAL(styleChanged(VLocalStyle*)), SIGNAL(styleChanged(VLocalStyle*)));
                pFeatureMenu->showInfo(_gO, _style, _gSIndex, i);
            }
            else if (_lE) {
                connect(pFeatureMenu, SIGNAL(lEndingChanged(VLineEnding*)), SIGNAL(lEndingChanged(VLineEnding*)));
                pFeatureMenu->showInfo(_lE, _gSIndex, i);
            }
            _pointFeatureMenus.push_back(pFeatureMenu);
            gShapeTreeView->addBranchWidget(pFeatureMenu, "Vertex " + QString::number(i + 1), "Vertices");
        }
        
        if (_gO)
            addRemoveVertexButtons.enableAddButton(_mw, ne_go_getBoundingBox(_gO), _pointFeatureMenus.size());
        else if (_lE)
            addRemoveVertexButtons.enableAddButton(_mw, ne_le_getBoundingBox(_lE), _pointFeatureMenus.size());
        
        if (_pointFeatureMenus.size() > 2)
            addRemoveVertexButtons.enableRemoveButton(_pointFeatureMenus.size());
    }
}

// LineFeatureMenu

LineFeatureMenu::LineFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    _line = NULL;
    lineTreeView = NULL;
    lineStartBranch = NULL;
    lineEndBranch = NULL;
    lineBasePoint1Branch = NULL;
    lineBasePoint2Branch = NULL;
    setLayout(&lineFeatureLayout);
    
    lineTreeView = new MyTreeView(parent);
    /// line features
    // start
    lineStartBranch = new MyGroupBox(lineTreeView);
    startDualSpinBox.setType("Position");
    startDualSpinBox.setRange(_mw);
    lineStartLayout.addWidget(&startDualSpinBox, lineStartLayout.rowCount(), 0);
    connect(&startDualSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeStart(const double&, const double&)));
    lineStartBranch->setLayout(&lineStartLayout);
    lineTreeView->addBranchWidget(lineStartBranch, "Start");
    // end
    lineEndBranch = new MyGroupBox(lineTreeView);
    endDualSpinBox.setType("Position");
    endDualSpinBox.setRange(_mw);
    lineEndLayout.addWidget(&endDualSpinBox, lineEndLayout.rowCount(), 0);
    connect(&endDualSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeEnd(const double&, const double&)));
    lineEndBranch->setLayout(&lineEndLayout);
    lineTreeView->addBranchWidget(lineEndBranch, "End");
    lineFeatureLayout.addWidget(lineTreeView, lineFeatureLayout.rowCount(), 0, 1, 2);
    
    
    // reset values
    resetValues();
}

void LineFeatureMenu::showInfo(LLineSegment* line) {
    // reset values
    resetValues();
    
    if (line) {
        _line = line;
        
        // start
        if (ne_ls_isSetStart(_line))
            startDualSpinBox.setCurrentValue(ne_point_getX(ne_ls_getStart(_line)), ne_point_getY(ne_ls_getStart(_line)));
        // end
        if (ne_ls_isSetEnd(_line))
            endDualSpinBox.setCurrentValue(ne_point_getX(ne_ls_getEnd(_line)), ne_point_getY(ne_ls_getEnd(_line)));
    }
}

void LineFeatureMenu::resetValues() {
    startDualSpinBox.resetValues();
    endDualSpinBox.resetValues();
    
    _line = NULL;
}

void LineFeatureMenu::collapseTree() {
    if (lineTreeView)
        lineTreeView->collapseAll();
}

void LineFeatureMenu::changeStart(const double& x, const double& y) {
    LPoint* point = ne_ls_getStart(_line);
    if (!ne_point_setX(point, x) && !ne_point_setY(point, y) && !ne_ls_setStart(_line, point)) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit lineChanged();
    }
}

void LineFeatureMenu::changeEnd(const double& x, const double& y) {
    LPoint* point = ne_ls_getEnd(_line);
    if (!ne_point_setX(point, x) && !ne_point_setY(point, y) && !ne_ls_setEnd(_line, point)) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit lineChanged();
    }
}

// CubicBezierFeatureMenu

CubicBezierFeatureMenu::CubicBezierFeatureMenu(QWidget* parent, MainWindow* mw) : LineFeatureMenu(parent, mw) {
    cubicBezierBasePoint1Branch = NULL;
    cubicBezierBasePoint2Branch = NULL;
    
    // basepoint1
    cubicBezierBasePoint1Branch = new MyGroupBox(lineTreeView);
    basePoint1DualSpinBox.setType("Position");
    basePoint1DualSpinBox.setRange(_mw);
    cubicBezierBasePoint1Layout.addWidget(&basePoint1DualSpinBox, cubicBezierBasePoint1Layout.rowCount(), 0);
    connect(&basePoint1DualSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint1(const double&, const double&)));
    cubicBezierBasePoint1Branch->setLayout(&cubicBezierBasePoint1Layout);
    lineTreeView->addBranchWidget(cubicBezierBasePoint1Branch, "BasePoint1");
    // basepoint2
    cubicBezierBasePoint2Branch = new MyGroupBox(lineTreeView);
    basePoint2DualSpinBox.setType("Position");
    basePoint2DualSpinBox.setRange(_mw);
    cubicBezierBasePoint2Layout.addWidget(&basePoint2DualSpinBox, cubicBezierBasePoint2Layout.rowCount(), 0);
    connect(&basePoint2DualSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint2(const double&, const double&)));
    cubicBezierBasePoint2Branch->setLayout(&cubicBezierBasePoint2Layout);
    lineTreeView->addBranchWidget(cubicBezierBasePoint2Branch, "BasePoint2");
    
    // reset values
    resetValues();
}

void CubicBezierFeatureMenu::showInfo(LLineSegment* line) {
    // reset values
    resetValues();
    
    if (line) {
        LineFeatureMenu::showInfo(line);
        // cubic bezier
        if (ne_ls_isCubicBezier(_line)) {
            // base point 1
            if (ne_cb_isSetBasePoint1(_line))
                basePoint1DualSpinBox.setCurrentValue(ne_point_getX(ne_cb_getBasePoint1(_line)), ne_point_getY(ne_cb_getBasePoint1(_line)));
            // base point 2
            if (ne_cb_isSetBasePoint2(_line))
                basePoint2DualSpinBox.setCurrentValue(ne_point_getX(ne_cb_getBasePoint2(_line)), ne_point_getY(ne_cb_getBasePoint2(_line)));
        }
    }
}

void CubicBezierFeatureMenu::resetValues() {
    LineFeatureMenu::resetValues();
    basePoint1DualSpinBox.resetValues();
    basePoint2DualSpinBox.resetValues();
}

void CubicBezierFeatureMenu::changeBasePoint1(const double& x, const double& y) {
    LPoint* point = ne_cb_getBasePoint1(_line);
    if (!ne_point_setX(point, x) && !ne_point_setY(point, y) && !ne_cb_setBasePoint1(_line, point)) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit lineChanged();
    }
}

void CubicBezierFeatureMenu::changeBasePoint2(const double& x, const double& y) {
    LPoint* point = ne_cb_getBasePoint2(_line);
    if (!ne_point_setX(point, x) && !ne_point_setY(point, y) && !ne_cb_setBasePoint2(_line, point)) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit lineChanged();
    }
}

// RenderPointFeatureMenu

RenderPointFeatureMenu::RenderPointFeatureMenu(QWidget* parent, MainWindow* mw) : MyGroupBox(parent) {
    _mw = mw;
    setLayout(&pointFeatureLayout);
    
    /// point
    // x label
    pointFeatureLayout.addWidget(new MyLabel("X:"), pointFeatureLayout.rowCount() - 1, 0, Qt::AlignLeft);
    // x
    pointFeatureLayout.addWidget(&pointXRelAbsSpinBox, pointFeatureLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&pointXRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeRPointX(const double&, const double&)));
    // y label
    pointFeatureLayout.addWidget(new MyLabel("Y:"), pointFeatureLayout.rowCount(), 0, Qt::AlignLeft);
    // y
    pointFeatureLayout.addWidget(&pointYRelAbsSpinBox, pointFeatureLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&pointYRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeRPointY(const double&, const double&)));

    // reset values
    resetValues();
}

void RenderPointFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex, const unsigned int& pointIndex) {
    _gSIndex = gSIndex;
    _pointIndex = pointIndex;
    
    if (gO && style) {
        _style = style;
        _gO = gO;
        if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex), _pointIndex))
            getInfoFromRenderPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex), _pointIndex));
        else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex), _pointIndex))
            getInfoFromRenderPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(_style), _gSIndex), _pointIndex));
    }
}

void RenderPointFeatureMenu::showInfo(VLineEnding* lE, const unsigned int& gSIndex, const unsigned int& pointIndex) {
    _gSIndex = gSIndex;
    _pointIndex = pointIndex;
    
    if (lE) {
        _lE = lE;
        if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex), _pointIndex))
            getInfoFromRenderPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex), _pointIndex));
        else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex), _pointIndex))
            getInfoFromRenderPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(_lE), _gSIndex), _pointIndex));
    }
}

void RenderPointFeatureMenu::getInfoFromRenderPoint(RenPoint* renderPoint) {
    if (renderPoint) {
        // x
        pointXRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
        pointXRelAbsSpinBox.setCurrentValue(renderPoint->getRPoint().x().a(), renderPoint->getRPoint().x().r());
        // y
        pointYRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
        pointYRelAbsSpinBox.setCurrentValue(renderPoint->getRPoint().y().a(), renderPoint->getRPoint().y().r());
    }
}

void RenderPointFeatureMenu::resetValues() {
    pointXRelAbsSpinBox.resetValues();
    pointYRelAbsSpinBox.resetValues();
    
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gSIndex = 0;
    _pointIndex = 0;
}

void RenderPointFeatureMenu::changeRPointX(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(abs, rel);
        RPoint * rp = NULL;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(*pX, ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->getRPoint().y());
                ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(*pX, ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->getRPoint().y());
                ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(*pX, ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->getRPoint().y());
                ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(*pX, ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->getRPoint().y());
                ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pX;
        delete rp;
    }
}

void RenderPointFeatureMenu::changeRPointY(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* pY = new RAVector(abs, rel);
        RPoint * rp = NULL;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->getRPoint().x(), *pY);
                ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->getRPoint().x(), *pY);
                ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->getRPoint().x(), *pY);
                ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->getRPoint().x(), *pY);
                ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)->setRPoint(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete pY;
        delete rp;
    }
}

// RenderCubicBezierFeatureMenu

RenderCubicBezierFeatureMenu::RenderCubicBezierFeatureMenu(QWidget* parent, MainWindow* mw) : RenderPointFeatureMenu(parent, mw) {
    /// basepoint1
    // x label
    pointFeatureLayout.addWidget(new MyLabel("BP1X:"), pointFeatureLayout.rowCount(), 0, Qt::AlignLeft);
    // x
    pointFeatureLayout.addWidget(&basePoint1XRelAbsSpinBox, pointFeatureLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&basePoint1XRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint1X(const double&, const double&)));
    // y label
    pointFeatureLayout.addWidget(new MyLabel("BP1Y:"), pointFeatureLayout.rowCount(), 0, Qt::AlignLeft);
    // y
    pointFeatureLayout.addWidget(&basePoint1YRelAbsSpinBox, pointFeatureLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&basePoint1YRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint1Y(const double&, const double&)));
    
    // basepoint2
    // x label
    pointFeatureLayout.addWidget(new MyLabel("BP2X:"), pointFeatureLayout.rowCount(), 0, Qt::AlignLeft);
    // x
    pointFeatureLayout.addWidget(&basePoint2XRelAbsSpinBox, pointFeatureLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&basePoint2XRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint2X(const double&, const double&)));
    // y label
    pointFeatureLayout.addWidget(new MyLabel("BP2Y:"), pointFeatureLayout.rowCount(), 0, Qt::AlignLeft);
    // y
    pointFeatureLayout.addWidget(&basePoint2YRelAbsSpinBox, pointFeatureLayout.rowCount() - 1, 1, Qt::AlignRight);
    connect(&basePoint2YRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint2Y(const double&, const double&)));
    
    // reset values
    resetValues();
    
    /*
    /// basepoint1
    pointBasePoint1Branch = new MyGroupBox(pointTreeView);
    // x label
    pointBasePoint1Layout.addWidget(new MyLabel("X:"), pointBasePoint1Layout.rowCount(), 1, Qt::AlignLeft);
    // x
    pointBasePoint1Layout.addWidget(&basePoint1XRelAbsSpinBox, pointBasePoint1Layout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&basePoint1XRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint1X(const double&, const double&)));
    // y label
    pointBasePoint1Layout.addWidget(new MyLabel("Y:"), pointBasePoint1Layout.rowCount(), 1, Qt::AlignLeft);
    // y
    pointBasePoint1Layout.addWidget(&basePoint1YRelAbsSpinBox, pointBasePoint1Layout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&basePoint1YRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint1Y(const double&, const double&)));
    pointBasePoint1Branch->setLayout(&pointBasePoint1Layout);
    pointTreeView->addBranchWidget(pointBasePoint1Branch, "BasePoint1");
    
    // basepoint2
    pointBasePoint2Branch = new MyGroupBox(pointTreeView);
    // x label
    pointBasePoint2Layout.addWidget(new MyLabel("X:"), pointBasePoint2Layout.rowCount(), 1, Qt::AlignLeft);
    // x
    pointBasePoint2Layout.addWidget(&basePoint2XRelAbsSpinBox, pointBasePoint2Layout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&basePoint2XRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint2X(const double&, const double&)));
    // y label
    pointBasePoint2Layout.addWidget(new MyLabel("Y:"), pointBasePoint2Layout.rowCount(), 1, Qt::AlignLeft);
    // y
    pointBasePoint2Layout.addWidget(&basePoint2YRelAbsSpinBox, pointBasePoint2Layout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&basePoint2YRelAbsSpinBox, SIGNAL(valueChanged(const double&, const double&)), this, SLOT(changeBasePoint2Y(const double&, const double&)));
    pointBasePoint2Branch->setLayout(&pointBasePoint2Layout);
    pointTreeView->addBranchWidget(pointBasePoint2Branch, "BasePoint2");
    
    // reset values
    resetValues();
     */
}

void RenderCubicBezierFeatureMenu::getInfoFromRenderPoint(RenPoint* renderPoint) {
    if (renderPoint) {
        RenderPointFeatureMenu::getInfoFromRenderPoint(renderPoint);
        if (ne_vrx_isRenderCubicBezier(renderPoint)) {
            /// base point 1
            // x
            basePoint1XRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
            basePoint1XRelAbsSpinBox.setCurrentValue(((RCubicBezier*)renderPoint)->getBasePoint1().x().a(), ((RCubicBezier*)renderPoint)->getBasePoint1().x().r());
            // y
            basePoint1YRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
            basePoint1YRelAbsSpinBox.setCurrentValue(((RCubicBezier*)renderPoint)->getBasePoint1().y().a(), ((RCubicBezier*)renderPoint)->getBasePoint1().y().r());
            
            /// base point 2
            // x
            basePoint2XRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
            basePoint2XRelAbsSpinBox.setCurrentValue(((RCubicBezier*)renderPoint)->getBasePoint2().x().a(), ((RCubicBezier*)renderPoint)->getBasePoint2().x().r());
            // y
            basePoint2YRelAbsSpinBox.setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
            basePoint2YRelAbsSpinBox.setCurrentValue(((RCubicBezier*)renderPoint)->getBasePoint2().y().a(), ((RCubicBezier*)renderPoint)->getBasePoint2().y().r());
        }
    }
}

void RenderCubicBezierFeatureMenu::resetValues() {
    RenderPointFeatureMenu::resetValues();
    basePoint1XRelAbsSpinBox.resetValues();
    basePoint1YRelAbsSpinBox.resetValues();
    basePoint2XRelAbsSpinBox.resetValues();
    basePoint2YRelAbsSpinBox.resetValues();
}

void RenderCubicBezierFeatureMenu::changeBasePoint1X(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* bp1X = new RAVector(abs, rel);
        RPoint * rp = NULL;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp1X, ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint1().y());
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp1X, ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint1().y());
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp1X, ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint1().y());
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp1X, ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint1().y());
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete bp1X;
        delete rp;
    }
}

void RenderCubicBezierFeatureMenu::changeBasePoint1Y(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* bp1Y = new RAVector(abs, rel);
        RPoint * rp = NULL;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint1().x(), *bp1Y);
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint1().x(), *bp1Y);
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint1().y(), *bp1Y);
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint1().y(), *bp1Y);
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint1(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete bp1Y;
        delete rp;
    }
}

void RenderCubicBezierFeatureMenu::changeBasePoint2X(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* bp2X = new RAVector(abs, rel);
        RPoint * rp = NULL;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp2X, ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint2().y());
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp2X, ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint2().y());
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp2X, ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint2().y());
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint(*bp2X, ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint2().y());
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete bp2X;
        delete rp;
    }
}

void RenderCubicBezierFeatureMenu::changeBasePoint2Y(const double& abs, const double& rel) {
    if (_mw && _mw->isSetSBMLDocument()) {
        RAVector* bp2Y = new RAVector(abs, rel);
        RPoint * rp = NULL;
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint2().x(), *bp2Y);
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->getBasePoint2().x(), *bp2Y);
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint2().y(), *bp2Y);
                ((RCubicBezier*)(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
            else if (ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)) {
                rp = new RPoint( ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->getBasePoint2().y(), *bp2Y);
                ((RCubicBezier*)(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), _gSIndex), _pointIndex)))->setBasePoint2(*rp);
                
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
        
        delete bp2Y;
        delete rp;
    }
}

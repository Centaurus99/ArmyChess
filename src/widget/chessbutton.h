#ifndef CHESSBUTTON_H
#define CHESSBUTTON_H

#include <QPushButton>

class ChessButton : public QPushButton {
    Q_OBJECT
signals:
    void chess_clicked(const int&);

private:
    // Button icon pixmap
    QPixmap icon_;
    // The number of this button
    int number_;

public:
    /// @param number The number of this button
    /// @param parent parent widget
    /// @param icon_name default icon
    ChessButton(const int& number, QWidget* parent = nullptr,
        const QString& icon_name = "");

    // Load icon from resource
    void loadIcon(const QString& res_name);
    // Set icon according to button size
    void setIcon();
};

#endif // CHESSBUTTON_H

#ifndef CHESSBUTTON_H
#define CHESSBUTTON_H

#include <QPushButton>

class ChessButton : public QPushButton {
private:
    // Button icon pixmap
    QPixmap icon_;

public:
    /// @param parent parent widget
    /// @param icon_name default icon
    ChessButton(QWidget* parent = nullptr, const QString& icon_name = "");

    // Load icon from resource
    void loadIcon(const QString& res_name);
    // Set icon according to button size
    void setIcon();
};

#endif // CHESSBUTTON_H

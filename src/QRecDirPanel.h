// Include necessary Qt headers
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDockWidget>

class TailScrollingLineEdit : public QLineEdit {
	Q_OBJECT

public:
	TailScrollingLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}

protected:
	void resizeEvent(QResizeEvent *event) override {
		QLineEdit::resizeEvent(event);
		// Move the cursor to the end so the line m_txtDirectory scrolls to show the tail.
		setCursorPosition(text().length());
	}
};

// Define a custom widget class that will be used as our dock panel.
// The Q_OBJECT macro is needed for Qt’s signals and slots.
class RecDirPanel : public QWidget {
	Q_OBJECT

public:
	explicit RecDirPanel(QWidget *parent = nullptr);

	~RecDirPanel();

public slots:
	// This slot opens a folder selection dialog.
	/**
	 * Open the folder selection dialog
	 */
	Q_SIGNAL void btnFolderBrowser_Click();

private:
	TailScrollingLineEdit *m_txtDirectory;
	QPushButton *m_btnFolderBrowser;

private:
	/**
	 * Update the obs config to the current textbox value
	 */
	void updateConfigToTextbox();

	/**
	 * Update the textbox to the config value
	 */
	void updateTextboxToConfig();

	/**
	 * Callback for OBS frontend events
	 * @param event Event Type
	 * @param private_data *RecDirPanel this reference
	 */
	static void obs_frontend_event_callback(enum obs_frontend_event event, void *private_data);
};
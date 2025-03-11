#include "QRecDirPanel.h"
#include "QRecDirPanel.moc"

#include <obs-frontend-api.h>
#include <util/config-file.h>
#include <plugin-support.h>

RecDirPanel::RecDirPanel(QWidget *parent) : QWidget(parent) {
	// Create a horizontal layout containing a text box and a browse button.
	QHBoxLayout *layout = new QHBoxLayout(this);
	m_txtDirectory = new TailScrollingLineEdit(this);
	m_btnFolderBrowser = new QPushButton("...", this);
	layout->addWidget(m_txtDirectory);
	layout->addWidget(m_btnFolderBrowser);

	// Connect the browse button to our slot.
	connect(m_btnFolderBrowser, &QPushButton::clicked, this, &RecDirPanel::btnFolderBrowser_Click);

	// Initialize the textbox with the current recording folder from OBS.
	obs_frontend_add_event_callback(&RecDirPanel::obs_frontend_event_callback, this);

	// Get the OBS configuration, section = AdvOut, item = RecFilePath
	config_t *config = obs_frontend_get_profile_config();
	const char *recFilePath = config_get_string(config, "AdvOut", "RecFilePath");

	if (recFilePath)
		m_txtDirectory->setText(recFilePath);

	// Set the focus leave textbox callback
	connect(m_txtDirectory, &TailScrollingLineEdit::editingFinished, [this]() { this->updateConfigToTextbox(); });
}

RecDirPanel::~RecDirPanel() {
	// Remove the frontend event
	obs_frontend_remove_event_callback(&RecDirPanel::obs_frontend_event_callback, this);
}

void RecDirPanel::btnFolderBrowser_Click() {
	// Open a dialog starting from the current path in the text box.
	QString dir = QFileDialog::getExistingDirectory(this, "Select Recording Directory", m_txtDirectory->text());
	if (!dir.isEmpty()) {
		// Update the text box.
		m_txtDirectory->setText(dir);

		// Update config
		this->updateConfigToTextbox();
	}
}

void RecDirPanel::updateConfigToTextbox() {
	auto text = m_txtDirectory->text();

	if (text.isEmpty())
		return;

	// Call the OBS frontend API to update the recording folder.
	config_t *config = obs_frontend_get_profile_config();
	config_set_string(config, "AdvOut", "RecFilePath", text.toUtf8().constData());
	config_save(config);
}

void RecDirPanel::updateTextboxToConfig() {
	config_t *config = obs_frontend_get_profile_config();
	const char *recordingPath = config_get_string(config, "AdvOut", "RecFilePath");

	QString qstrRecPath(recordingPath);
	this->m_txtDirectory->setText(qstrRecPath);
}

void RecDirPanel::obs_frontend_event_callback(enum obs_frontend_event event, void *private_data) {
	obs_log(LOG_INFO, "obs_frontend_event_callback: %d", event);

	RecDirPanel *pRec = static_cast<RecDirPanel *>(private_data);
	if (pRec && event == OBS_FRONTEND_EVENT_PROFILE_CHANGED)
		pRec->updateTextboxToConfig();
}

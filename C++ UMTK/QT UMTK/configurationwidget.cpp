#include "configurationwidget.h"

ConfigurationWidget::ConfigurationWidget()
{
    close_button = new QPushButton();
    company_table = new QTableView();

    c_insert_button = new QPushButton();
    c_ou_edit = new QLineEdit();
    c_company_edit = new QLineEdit();
    c_prefix_edit = new QLineEdit();
    c_sam_combo = new QComboBox();
    c_enable_button = new QCheckBox();

    folder_redirection_edit = new QLineEdit();
    profile_storage_edit = new QLineEdit();

    user_creation_text_edit = new QLineEdit();
    user_disable_text_edit = new QLineEdit();
    image_path_edit = new QLineEdit();
    company_logo_position = new QComboBox();

    display_ou_prefix = new QCheckBox();
    display_employee_acl = new QCheckBox();

    MultiCompanySettings = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Elysian Productions\\UMTK-Classic\\Company Settings\\", QSettings::Registry64Format);
    SamGenerationSettings = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Elysian Productions\\UMTK-Classic\\Generation Settings\\SamAccount Settings\\", QSettings::Registry64Format);
    PDFSettings = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Elysian Productions\\UMTK-Classic\\PDF Settings\\", QSettings::Registry64Format);
    DisableSettings = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Elysian Productions\\UMTK-Classic\\Disable Settings\\", QSettings::Registry64Format);
    AdvancedSortDisplaySettings = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Elysian Productions\\UMTK-Classic\\Advanced Sort and Display\\", QSettings::Registry64Format);
    if(initalize_database("C:\\Program Files (x86)\\UMTK-Classic\\Database\\UMTK.db"))
    {
        model = new QSqlQueryModel(this);
        model->setQuery("SELECT * FROM Clients");
        company_table->setModel(model);
    }
    else
    {

    }
}

ConfigurationWidget::~ConfigurationWidget()
{
    delete model;
    //delete MultiCompanySettings;

}

bool ConfigurationWidget::initalize_database(const QString &db_path)
{
     database_name = db_path;
     auto db = QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName(database_name);
     bool is_open = db.open();
     return is_open;
}

QSqlDatabase ConfigurationWidget::get_database()
{
    return QSqlDatabase::database(database_name);
}

QWidget* ConfigurationWidget::get_widget()
{
    return get_menu_widget(close_button);
}

QWidget* ConfigurationWidget::get_menu_widget(QPushButton *close_button)
{
    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();
    QHBoxLayout *menus_layout = new QHBoxLayout();
    QVBoxLayout *secondary_layout = new QVBoxLayout();
    QVBoxLayout *tertiary_layout = new QVBoxLayout();

    menus_widget = new QStackedWidget();

    menu_options_widget = new QListWidget();
    menu_options_widget->setViewMode(QListView::ListMode);
    menu_options_widget->setMovement(QListView::Static);
    menu_options_widget->setMaximumWidth(150);
    menu_options_widget->setSpacing(6);

    QListWidgetItem *company_item = new QListWidgetItem(menu_options_widget);
    company_item->setText("Company setup");
    company_item->setTextAlignment(Qt::AlignHCenter);
    company_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *pdf_item = new QListWidgetItem(menu_options_widget);
    pdf_item->setText("Customize PDFs");
    pdf_item->setTextAlignment(Qt::AlignHCenter);
    pdf_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *generation_item = new QListWidgetItem(menu_options_widget);
    generation_item->setText("Generation settings");
    generation_item->setTextAlignment(Qt::AlignHCenter);
    generation_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    QListWidgetItem *sortdisplay_item = new QListWidgetItem(menu_options_widget);
    sortdisplay_item->setText("Sort & Display");
    sortdisplay_item->setTextAlignment(Qt::AlignHCenter);
    sortdisplay_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    /*QListWidgetItem *disable_item = new QListWidgetItem(menu_options_widget);
    disable_item->setText("Disable settings");
    disable_item->setTextAlignment(Qt::AlignHCenter);
    disable_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);*/

    QListWidgetItem *help_item = new QListWidgetItem(menu_options_widget);
    help_item->setText("Help");
    help_item->setTextAlignment(Qt::AlignHCenter);
    help_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    menus_widget->insertWidget(0, get_company_custimization_widget(c_insert_button, c_ou_edit, c_company_edit, c_prefix_edit, c_sam_combo));
    menus_widget->insertWidget(1, get_pdf_custimization_widget(user_creation_text_edit, user_disable_text_edit, image_path_edit));
    menus_widget->insertWidget(2, get_generation_custimization_widget());
    menus_widget->insertWidget(3, get_advanced_sortdisplay_widget(display_ou_prefix, display_employee_acl));
    //menus_widget->insertWidget(3, get_disable_custimation_widget());
    menus_widget->insertWidget(4, getHelpWidget());


    close_button->setText("Close configuration");
    close_button->setToolTip("Close the configuration page and return to the primary menu.");


    menus_layout->addWidget(menu_options_widget);

    secondary_layout->addWidget(close_button);

    tertiary_layout->addWidget(menus_widget);


    primary_layout->addLayout(menus_layout, 0, 0);
    primary_layout->addLayout(tertiary_layout, 0, 1);
    primary_layout->addLayout(secondary_layout, 1, 0);
    primary_display->setLayout(primary_layout);

    connect(menu_options_widget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(swap_menu(QListWidgetItem*, QListWidgetItem*)));

    return primary_display;
}

QWidget* ConfigurationWidget::get_company_custimization_widget(QPushButton *c_insert_button, QLineEdit *c_ou_edit, QLineEdit *c_company_edit, QLineEdit *c_prefix_edit, QComboBox *c_sam_combo)
{

    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();

    QLabel *company_label = new QLabel("Company settings");
    company_label->setFrameShape(QFrame::Panel);
    company_label->setFrameShadow(QFrame::Sunken);
    company_label->setStyleSheet("background-color:white");
    company_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    QLabel *info_label = new QLabel("In order to enable multi-company support you must check the 'Enable Multi-Company mode'.\nOnce the box has been checked you will be able to create and insert data into the database.");
    info_label->setFrameShape(QFrame::Panel);
    info_label->setFrameShadow(QFrame::Sunken);
    info_label->setStyleSheet("background-color:white");
    info_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);


    c_insert_button->setText("Insert data");
    c_insert_button->setToolTip("Fill in all fields and click this button to add the data to your sql db.");
    c_ou_edit->setPlaceholderText("Enter OU CN");
    c_company_edit->setPlaceholderText("Enter company name");
    c_prefix_edit->setPlaceholderText("Enter user prefix");
    QStringList sam_options = {"Select SamAccount style", "First name", "Last name", "First initial last name", "Last name first initial", "First name last name", "Last name first name"};
    c_sam_combo->addItems(sam_options);
    c_enable_button->setText("Enable Multi-Company mode");
    c_enable_button->setToolTip("Enabling multiple companies will result in the domain dropdown being empty\nuntill you fill in the database.");
    QSpacerItem *spacer_one = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    primary_layout->setSpacing(1);
    primary_layout->setHorizontalSpacing(0);
    primary_layout->setVerticalSpacing(1);
    primary_layout->addWidget(company_label, 0, 0);
    primary_layout->addWidget(company_table, 1, 0);
    primary_layout->addWidget(c_ou_edit, 3, 0);
    primary_layout->addWidget(c_company_edit, 4, 0);
    primary_layout->addWidget(c_prefix_edit, 5, 0);
    primary_layout->addWidget(c_sam_combo, 6, 0);
    primary_layout->addWidget(c_insert_button, 7, 0);
    primary_layout->addWidget(c_enable_button, 8, 0);
    primary_layout->addItem(spacer_one , 9, 0);
    primary_layout->addWidget(info_label, 10, 0);
    primary_display->setLayout(primary_layout);

    if(MultiCompanySettings->value("MultiCompanyEnabled").toInt() == 1)
    {
        c_enable_button->setChecked(true);
        c_insert_button->setVisible(true);
    }
    else if(MultiCompanySettings->value("MultiCompanyEnabled").toInt() == 0)
    {
        c_enable_button->setChecked(false);
        c_insert_button->setVisible(false);
    }

    connect(c_sam_combo, qOverload<int>(&QComboBox::currentIndexChanged), [=] (int var) { Q_EMIT(setSamSetting(var));});
    connect(c_insert_button, &QPushButton::clicked, this, &ConfigurationWidget::setupQuery);
    connect(c_enable_button, &QCheckBox::toggled, [=] (bool var) {Q_EMIT(setMultiCompanyStatus(var));});

    return primary_display;
}

QWidget* ConfigurationWidget::get_pdf_custimization_widget(QLineEdit *user_creation_text_edit, QLineEdit *user_disable_text_edit, QLineEdit *image_path_edit)
{
    QGridLayout *primary_layout = new QGridLayout();
    QWidget *primary_display = new QWidget();

    QLabel *new_user_label = new QLabel("New user text:");
    QLabel *disable_user_label = new QLabel("Disable user text:");
    QLabel *image_path_label = new QLabel("Image path:");
    QSpacerItem *spacer_one = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QLabel *pdf_settings_label = new QLabel("Customize PDF elements");

    pdf_settings_label->setFrameShape(QFrame::Panel);
    pdf_settings_label->setFrameShadow(QFrame::Sunken);
    pdf_settings_label->setStyleSheet("background-color:white");
    pdf_settings_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    user_creation_text_edit->setPlaceholderText("Type text");
    user_creation_text_edit->setToolTip("Type the text to be printed at the top of the new user PDF generated when a user has been created.\nOnce done, just hit enter.");

    user_disable_text_edit->setPlaceholderText("Type text");
    user_disable_text_edit->setToolTip("Type the text to be printed on the disable user PDF generated when a user has been disabled.\nOnce done, just hit enter.");

    image_path_edit->setPlaceholderText("Type path");
    image_path_edit->setToolTip("Type the full path to the image you want to be displayed at the bottom of the new user and disabled user PDFs.\nOnce done, hit enter.");

    QStringList logo_positions = {"Select logo position", "Top", "Bottom"};
    company_logo_position->addItems(logo_positions);
    company_logo_position->setToolTip("If you add a logo you can choose to add it to either the bottom or top of the page.");

    primary_layout->addWidget(pdf_settings_label, 0, 0);
    primary_layout->addWidget(new_user_label, 1, 0);
    primary_layout->addWidget(user_creation_text_edit, 1, 1);
    primary_layout->addWidget(disable_user_label, 2, 0);
    primary_layout->addWidget(user_disable_text_edit, 2, 1);
    primary_layout->addWidget(image_path_label, 3, 0);
    primary_layout->addWidget(image_path_edit, 3, 1);
    primary_layout->addWidget(company_logo_position, 4, 0);
    primary_layout->addItem(spacer_one, 5, 0);
    primary_display->setLayout(primary_layout);


    if(PDFSettings->value("UserCreationText").toString().length() > 0)
    {
        user_creation_text_edit->setPlaceholderText(PDFSettings->value("UserCreationText").toString());
    }
    if(PDFSettings->value("UserDisableText").toString().length() > 0)
    {
        user_disable_text_edit->setPlaceholderText(PDFSettings->value("UserDisableText").toString());
    }
    if(PDFSettings->value("CompanyLogoPath").toString().length() > 0 && PDFSettings->value("CompanyLogoPath").toString() == "Top" || PDFSettings->value("CompanyLogoPath").toString() == "Bottom")
    {
        image_path_edit->setPlaceholderText(PDFSettings->value("CompanyLogoPath").toString());
        if(PDFSettings->value("LogoPosition").toString() == "Top")
        {
            company_logo_position->setCurrentIndex(1);
        }
        else if(PDFSettings->value("LogoPosition").toString() == "Bottom")
        {
            company_logo_position->setCurrentIndex(2);
        }
    }

    connect(user_creation_text_edit, &QLineEdit::returnPressed, this, &ConfigurationWidget::setNewUserText);
    connect(user_disable_text_edit, &QLineEdit::returnPressed, this, &ConfigurationWidget::setDisableUserText);
    connect(image_path_edit, &QLineEdit::returnPressed, this, &ConfigurationWidget::setImagePath);
    connect(company_logo_position, qOverload<int>(&QComboBox::currentIndexChanged), [=] (int var) {Q_EMIT(setLogoPosition(var));});

    return primary_display;
}

QWidget* ConfigurationWidget::get_generation_custimization_widget()
{
    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();

    // SamAccountSettings
    QSpacerItem *spacer_one = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QLabel *sam_label = new QLabel("SamAccountName Settings");
    sam_label->setFrameShape(QFrame::Panel);
    sam_label->setFrameShadow(QFrame::Sunken);
    sam_label->setStyleSheet("background-color:white");
    sam_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    QStringList sam_options {"SamAccountName Styles", "First name (Example)", "Last name (Name)", "First initial, Last name (eName)",
                            "Last initial, First name (nExample)", "FirstName, LastName (ExampleName)", "Last name, First name (NameExample)"};
    QComboBox *sam_styles = new QComboBox();
    sam_styles->addItems(sam_options);

    // Ticketing settings
    QLabel *ticketing_label = new QLabel("Ticketing settings");
    ticketing_label->setFrameShape(QFrame::Panel);
    ticketing_label->setFrameShadow(QFrame::Sunken);
    ticketing_label->setStyleSheet("background-color:white");
    ticketing_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    QSpacerItem *spacer_two = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QCheckBox *ticketing_button_on = new QCheckBox();
    QCheckBox *ticketing_button_off = new QCheckBox();
    QButtonGroup *ticketing_on_off_group = new QButtonGroup();
    ticketing_button_on->setText("Turn ticketing on");
    ticketing_button_off->setText("Turn ticketing off");
    ticketing_on_off_group->addButton(ticketing_button_on);
    ticketing_on_off_group->addButton(ticketing_button_off);


    // SMTP settings
    QLabel *smtp_settings_label = new QLabel("SMTP settings");
    smtp_settings_label->setFrameShape(QFrame::Panel);
    smtp_settings_label->setFrameShadow(QFrame::Sunken);
    smtp_settings_label->setStyleSheet("background-color:white");
    smtp_settings_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    QLabel *smtp_email_label = new QLabel("SMTP Email");
    QLineEdit *smtp_email_edit = new QLineEdit();
    smtp_email_edit->setPlaceholderText("Enter email address");
    smtp_email_edit->setToolTip("Enter the email address you will authenticate with.");
    QLabel *smtp_credential_label = new QLabel("Credentials");
    QLineEdit *smtp_credential_edit = new QLineEdit();
    smtp_credential_edit->setPlaceholderText("Enter password");
    smtp_credential_edit->setToolTip("Enter the password you will authenticate with.");
    smtp_credential_edit->setEchoMode(QLineEdit::Password);
    QStringList security_protocols = {"Security protocols", "SSL\\TLS", "STARTTLS", "Plain Text"};
    QComboBox *smtp_protocol_combo = new QComboBox();
    smtp_protocol_combo->addItems(security_protocols);
    smtp_protocol_combo->setToolTip("Select the security protocol you will need for authentication.");
    QLabel *smtp_incport_label = new QLabel("Incoming port");
    QLineEdit *smtp_incport_edit = new QLineEdit();
    smtp_incport_edit->setPlaceholderText("Enter mail servers incoming smtp port");
    smtp_incport_edit->setToolTip("Enter your servers incoming smtp port here.");
    QLabel *smtp_outport_label = new QLabel("Outgoing port");
    QLineEdit *smtp_outport_edit = new QLineEdit();
    smtp_outport_edit->setPlaceholderText("Enter mail servers outgoing smtp port");
    smtp_outport_edit->setToolTip("Enter your servers outgoing smtp port here.");
    QLabel *smtp_sub_label = new QLabel("Generic email subject");
    QLineEdit *smtp_sub_edit = new QLineEdit();
    smtp_sub_edit->setPlaceholderText("Enter generic subject");
    smtp_sub_edit->setToolTip("Enter a generic subject for your email.\nNote: If you enable the ticketing feature, the ticket number will be prepended to the email subject.");
    QSpacerItem *spacer_three = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    //

    primary_layout->setSpacing(1);
    primary_layout->setHorizontalSpacing(0);
    primary_layout->setVerticalSpacing(1);
    primary_layout->addWidget(sam_label, 0, 0);
    primary_layout->addWidget(sam_styles, 1, 0);
    primary_layout->addItem(spacer_one , 3, 0);
    primary_layout->addWidget(ticketing_label, 4, 0);
    primary_layout->addWidget(ticketing_button_on, 5, 0);
    primary_layout->addWidget(ticketing_button_off, 5, 1);
    primary_layout->addItem(spacer_two, 6, 0);
    primary_layout->addWidget(smtp_settings_label, 7, 0);
    primary_layout->addWidget(smtp_email_label, 8, 0);
    primary_layout->addWidget(smtp_email_edit, 8, 1);
    primary_layout->addWidget(smtp_credential_label, 9, 0);
    primary_layout->addWidget(smtp_credential_edit, 9, 1);
    primary_layout->addWidget(smtp_incport_label, 10, 0);
    primary_layout->addWidget(smtp_incport_edit, 10, 1);
    primary_layout->addWidget(smtp_outport_label, 11, 0);
    primary_layout->addWidget(smtp_outport_edit, 11, 1);
    primary_layout->addWidget(smtp_sub_label, 12, 0);
    primary_layout->addWidget(smtp_sub_edit, 12, 1);
    primary_layout->addWidget(smtp_protocol_combo, 13, 0);
    primary_layout->addItem(spacer_three, 14, 0);
    primary_display->setLayout(primary_layout);


  if(SamGenerationSettings->value("SamStyle").toInt() > 0)
  {
      sam_styles->setCurrentIndex(SamGenerationSettings->value("SamStyle").toInt());
  }

    connect(sam_styles, qOverload<int>(&QComboBox::currentIndexChanged), [=] (int var) {Q_EMIT(setSingleEnvSamStyle(var));});
    return primary_display;
}

QWidget* ConfigurationWidget::get_disable_custimation_widget()
{
    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();
    QCheckBox *turn_on_cleanup = new QCheckBox();
    QSpacerItem *spacer_one = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QLabel *cleanup_label = new QLabel("Disable User settings");
    QLabel *folder_redirection_label = new QLabel("Folder redirection share");
    QLabel *profile_storage_label = new QLabel("Storage location for profiles");
    QLabel *profile_cleanup_label = new QLabel("Profile settings");

    turn_on_cleanup->setText("Turn on profile cleanup");


    cleanup_label->setFrameShape(QFrame::Panel);
    cleanup_label->setFrameShadow(QFrame::Sunken);
    cleanup_label->setStyleSheet("background-color:white");
    cleanup_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    profile_cleanup_label->setFrameShape(QFrame::Panel);
    profile_cleanup_label->setFrameShadow(QFrame::Sunken);
    profile_cleanup_label->setStyleSheet("background-color:white");
    profile_cleanup_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);



    if(DisableSettings->value("CleanupOn").toInt() == 1)
    {
        turn_on_cleanup->setChecked(true);
        if(DisableSettings->value("RedirectionShare").toString().length() > 0 && DisableSettings->value("StorageLocation").toString().length() > 0)
        {
            folder_redirection_edit->setPlaceholderText(DisableSettings->value("RedirectionShare").toString());
            folder_redirection_edit->setToolTip("Input the current path to your folder redirection share.\nOnce done, just hit enter on your keyboard.");

            profile_storage_edit->setPlaceholderText(DisableSettings->value("StorageLocation").toString());
            profile_storage_edit->setToolTip("Input the path to the share where you store old profiles of former employees.\nOnce done, just hit enter on your keyboard.");
        }
        else if(DisableSettings->value("RedirectionShare").toString().length() <= 0 || DisableSettings->value("StorageLocation").toString().length() <= 0)
        {
            folder_redirection_edit->setPlaceholderText("Enter folder redirection share");
            folder_redirection_edit->setToolTip("Input the current path to your folder redirection share.\nOnce done, just hit enter on your keyboard.");

            profile_storage_edit->setPlaceholderText("Enter the path to the share where you store profiles.");
            profile_storage_edit->setToolTip("Input the path to the share where you store old profiles of former employees.\nOnce done, just hit enter on your keyboard.");
        }
    }
    else if(DisableSettings->value("CleanupOn").toInt() == 0)
    {
        folder_redirection_edit->setPlaceholderText("Enter folder redirection share");
        folder_redirection_edit->setToolTip("Input the current path to your folder redirection share.\nOnce done, just hit enter on your keyboard.");

        profile_storage_edit->setPlaceholderText("Enter the path to the share where you store profiles.");
        profile_storage_edit->setToolTip("Input the path to the share where you store old profiles of former employees.\nOnce done, just hit enter on your keyboard.");
    }



    primary_layout->setSpacing(1);
    primary_layout->setHorizontalSpacing(0);
    primary_layout->setVerticalSpacing(1);
    primary_layout->addWidget(cleanup_label, 0, 0);
    primary_layout->addWidget(turn_on_cleanup, 1, 0);
    primary_layout->addWidget(profile_cleanup_label, 3, 0);
    primary_layout->addWidget(folder_redirection_label, 4, 0);
    primary_layout->addWidget(folder_redirection_edit, 4, 1);
    primary_layout->addWidget(profile_storage_label, 5, 0);
    primary_layout->addWidget(profile_storage_edit, 5, 1);
    primary_layout->addItem(spacer_one, 6, 0);

    connect(turn_on_cleanup, &QCheckBox::toggled, [=] (bool isChecked) { Q_EMIT(setProfileCleanupSettings(isChecked)); });
    connect(folder_redirection_edit, &QLineEdit::returnPressed, this, &ConfigurationWidget::setProfileRedirectionPath);
    connect(profile_storage_edit, &QLineEdit::returnPressed, this, &ConfigurationWidget::setProfileStoragePath);


    primary_display->setLayout(primary_layout);
    return primary_display;
}

QWidget* ConfigurationWidget::getHelpWidget()
{
    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();
    QStandardItemModel *model = new QStandardItemModel();
    QStandardItem *company_setup = new QStandardItem("Multi-Company Setup");
    QStandardItem *database_child = new QStandardItem("Database help");
    QStandardItem *samaccountstyle_child = new QStandardItem("SamAccount_styles");
    database_child->setEditable(false);
    samaccountstyle_child->setEditable(false);
    company_setup->setChild(0, 0, database_child);
    company_setup->setChild(1, 0, samaccountstyle_child);

    QStandardItem *pdf_custimization = new QStandardItem("PDF Custimization");
    QStandardItem *newuser_child = new QStandardItem("New user text");
    QStandardItem *disableuser_child = new QStandardItem("Disable user text");
    QStandardItem *imagepath_child = new QStandardItem("Image path");
    newuser_child->setEditable(false);
    disableuser_child->setEditable(false);
    imagepath_child->setEditable(false);
    pdf_custimization->setChild(0, 0, newuser_child);
    pdf_custimization->setChild(1, 0, disableuser_child);
    pdf_custimization->setChild(1, 0, imagepath_child);

    QStandardItem *generation_settings = new QStandardItem("Generation Settings");
    QStandardItem *samaccount_child = new QStandardItem("SamAccountName settings");
    QStandardItem *ticketing_child = new QStandardItem("Ticketing settings");
    QStandardItem *smtp_child = new QStandardItem("SMTP settings");
    samaccount_child->setEditable(false);
    ticketing_child->setEditable(false);
    smtp_child->setEditable(false);
    generation_settings->setChild(0, 0, samaccount_child);
    generation_settings->setChild(1, 0, ticketing_child);
    generation_settings->setChild(2, 0, smtp_child);
    //generation_settings->setChild(2, 0, new QStandardItem("SMTP settings"));


    QStandardItem *disable_settings = new QStandardItem("Disable Settings");
    QStandardItem *profile_cleanup_child = new QStandardItem("Disable user settings");
    QStandardItem *profile_settings_child = new QStandardItem("Profile settings");
    profile_cleanup_child->setEditable(false);
    profile_settings_child->setEditable(false);
    disable_settings->setChild(0, 0, profile_cleanup_child);
    disable_settings->setChild(1, 0, profile_settings_child);



    model->setColumnCount(1);
    model->setRowCount(4);
    model->setHorizontalHeaderItem(0, new QStandardItem("Select option"));
    model->setItem(0, 0, company_setup);
    model->setItem(1, 0, pdf_custimization);
    model->setItem(2, 0, generation_settings);
    model->setItem(3, 0, disable_settings);

    //model->setHorizontalHeaderItem(0, new QStandardItem("Company Setup Help"));
    //model->setHorizontalHeaderItem(0, new QStandardItem("PDF Custimization Help"));
    //model->setHorizontalHeaderItem(0, new QStandardItem("Auto-Generation Help"));
    //model->setHorizontalHeaderItem(0, new QStandardItem("Disable Settings Help"));
    QTreeView *help_tree = new QTreeView();
    help_tree->setModel(model);



    QTextBrowser *browser = new QTextBrowser();



    primary_layout->addWidget(help_tree, 0, 0);
    primary_layout->addWidget(browser, 1, 0);
    primary_display->setLayout(primary_layout);
    return primary_display;
    //QTreeView
}

QWidget* ConfigurationWidget::get_advanced_sortdisplay_widget(QCheckBox *display_ou_prefix, QCheckBox *display_employee_acl)
{
    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();

    QSpacerItem *spacer_one = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QLabel *ou_label = new QLabel("Organizational Employee name prefix");
    ou_label->setFrameShape(QFrame::Panel);
    ou_label->setFrameShadow(QFrame::Sunken);
    ou_label->setStyleSheet("background-color:white");
    ou_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    display_ou_prefix->setToolTip("Checking this box will display 'OU - Employee name'.");


    QSpacerItem *spacer_two = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QLabel *acl_label = new QLabel("Display Employee ACLs");
    acl_label->setFrameShape(QFrame::Panel);
    acl_label->setFrameShadow(QFrame::Sunken);
    acl_label->setStyleSheet("background-color:white");
    acl_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    display_employee_acl->setToolTip("Checking this box will pull all ACLs for a user and display them in a widget.");


    if(AdvancedSortDisplaySettings->value("OU Prefix").toInt() > 0)
    {
        display_ou_prefix->setChecked(true);
    }

    if(AdvancedSortDisplaySettings->value("Display ACL").toInt() > 0)
    {
        display_employee_acl->setChecked(true);
    }


    connect(display_ou_prefix, &QCheckBox::toggled, [=] (bool var) {Q_EMIT(setDisplayOUPrefixSetting(var));});
    connect(display_employee_acl, &QCheckBox::toggled, [=] (bool var) {Q_EMIT(setDisplayEmployeeACLSetting(var));});



    primary_layout->setSpacing(1);
    primary_layout->setHorizontalSpacing(0);
    primary_layout->setVerticalSpacing(1);
    primary_layout->addWidget(ou_label, 0, 0);
    primary_layout->addWidget(display_ou_prefix, 1, 0);
    primary_layout->addItem(spacer_one, 2, 0);
    primary_layout->addWidget(acl_label, 3, 0);
    primary_layout->addWidget(display_employee_acl, 4, 0);
    primary_layout->addItem(spacer_two, 5, 0);


    primary_display->setLayout(primary_layout);
    return primary_display;

}

void ConfigurationWidget::setupQuery()
{
    setOUCNName(c_ou_edit->text());
    setCompanyName(c_company_edit->text());
    setUserPrefix(c_prefix_edit->text());
    runQuery(ou_name(), company_name(), prefix_name(), sam_setting());
}

void ConfigurationWidget::refresh_model()
{
    if(initalize_database("C:\\Program Files (x86)\\UMTK-Classic\\Database\\UMTK.db"))
    {
        model->setQuery("SELECT * FROM Clients");
    }
}

void ConfigurationWidget::runQuery(const QString &OU, const QString &company, const QString &prefix, const int &sam_selection)
{
    if(OU.length() > 0 && company.length() > 0 && prefix.length() > 0 && sam_selection > 0)
    {
        if(initalize_database("C:\\Program Files (x86)\\UMTK-Classic\\Database\\UMTK.db"))
        {
            QSqlQuery *query = new QSqlQuery(get_database());
            query->prepare("INSERT INTO Clients (OU, Company, Prefix, SamAccount)"
                           "Values (:OU, :Company, :Prefix, :SamAccount)");
            query->bindValue(":OU", ou_name());
            query->bindValue(":Company", company_name());
            query->bindValue(":Prefix", prefix_name());
            query->bindValue(":SamAccount", sam_setting());
            query->exec();
            refresh_model();
        }
        else
        {

        }
    }
    if(OU.length() > 0 && company.length() > 0 && prefix.length() > 0)
    {

        if(initalize_database("C:\\Program Files (x86)\\UMTK-Classic\\Database\\UMTK.db"))
        {
            QSqlQuery *query = new QSqlQuery(get_database());
            query->prepare("INSERT INTO Clients (OU, Company, Prefix)"
                           "Values (:OU, :Company, :Prefix)");
            query->bindValue(":OU", ou_name());
            query->bindValue(":Company", company_name());
            query->bindValue(":Prefix", prefix_name());
            query->exec();
        }
        else
        {

        }

    }
    if(OU.length() > 0 && company.length() > 0 && sam_selection > 0)
    {

        if(initalize_database("C:\\Program Files (x86)\\UMTK-Classic\\Database\\UMTK.db"))
        {
            QSqlQuery *query = new QSqlQuery(get_database());
            query->prepare("INSERT INTO Clients (OU, Company, SamAccount)"
                           "Values (:OU, :Company, :SamAccount)");
            query->bindValue(":OU", ou_name());
            query->bindValue(":Company", company_name());
            query->bindValue(":SamAccount", sam_setting());
            query->exec();
        }
        else
        {

        }
    }
    if(OU.length() <= 0 || company.length() <= 0)
    {
        if(OU.length() <= 0)
        {
            c_ou_edit->setStyleSheet("background-color:red");
            c_ou_edit->setText("This parameter cannot be empty.");
        }
        if(company.length() <= 0)
        {
            c_company_edit->setStyleSheet("background-color:red");
            c_company_edit->setText("This parameter cannot be empty.");
        }
    }
}

void ConfigurationWidget::setSamSetting(const int &samsetting)
{
    if(samsetting != _samsetting)
    {
        _samsetting = samsetting;
    }
}

void ConfigurationWidget::swap_menu(QListWidgetItem *selected, QListWidgetItem *previous)
{
  if(!selected)
  {
      selected = previous;
  }
  menus_widget->setCurrentIndex(menu_options_widget->row(selected));
}

void ConfigurationWidget::setCompanyName(const QString &company)
{
    if(company != _company)
    {
        _company = company;
    }
}

void ConfigurationWidget::setOUCNName(const QString &cn)
{
    if(cn != _oucn)
    {
        _oucn = cn;
    }
}

void ConfigurationWidget::setUserPrefix(const QString &prefix)
{
    if(prefix != _prefix)
    {
        _prefix = prefix;
    }
}

void ConfigurationWidget::setSingleEnvSamStyle(const int &option)
{
    // Write to RegKey HKLM "Software\Elysian Productions\UMTK-Classic\Generation Settings\SamAccount Settings\" "SamStyle" "option" (DWORD)
    if(option == 1)
    {
        SamGenerationSettings->setValue("SamStyle", 1);
    }
    if(option == 2)
    {
        SamGenerationSettings->setValue("SamStyle", 2);
    }
    if(option == 3)
    {
        SamGenerationSettings->setValue("SamStyle", 3);
    }
    if(option == 4)
    {
        SamGenerationSettings->setValue("SamStyle", 4);
    }
    if(option == 5)
    {
        SamGenerationSettings->setValue("SamStyle", 5);
    }
    if(option == 6)
    {
        SamGenerationSettings->setValue("SamStyle", 6);
    }

}

void ConfigurationWidget::setMultiCompanyStatus(const bool &status)
{
    if(status)
    {
        // Enable multi company support
        MultiCompanySettings->setValue("MultiCompanyEnabled", 1);
        // Write to RegKey HKLM "Software\Elysian Productions\UMTK-Classic\Company Settings\" "MultiCompanyEnabled" "0" (DWORD) -> 1
        c_enable_button->setChecked(true);
        c_insert_button->setVisible(true);
    }
    else if(!status)
    {
        // Disable multi company support
        MultiCompanySettings->setValue("MultiCompanyEnabled", 0);
        // Write to RegKey HKLM "Software\Elysian Productions\UMTK-Classic\Company Settings\" "MultiCompanyEnabled" "0" (DWORD) -> 0
        c_enable_button->setChecked(false);
        c_insert_button->setVisible(false);
    }
}

void ConfigurationWidget::setDisplayOUPrefixSetting(const bool &var)
{
    if(var == true)
    {
        AdvancedSortDisplaySettings->setValue("OU Prefix", 1);
        display_ou_prefix->setChecked(true);
        // WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Advanced Sort and Display\" "OU Prefix" "1"

    }
    else if(var == false)
    {
        AdvancedSortDisplaySettings->setValue("OU Prefix", 0);
        display_ou_prefix->setChecked(false);
        // WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Advanced Sort and Display\" "OU Prefix" "0"
    }
}

void ConfigurationWidget::setDisplayEmployeeACLSetting(const bool &var)
{
    if(var == true)
    {
        AdvancedSortDisplaySettings->setValue("Display ACL", 1);
        display_employee_acl->setChecked(true);
        // WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Advanced Sort and Display\" "Display ACL" "1"
    }
    else if(var == false)
    {
        AdvancedSortDisplaySettings->setValue("Display ACL", 0);
        display_employee_acl->setChecked(false);
        // WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Advanced Sort and Display\" "Display ACL" "0"
    }
}

void ConfigurationWidget::setNewUserText()
{
    // WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "UserCreationText" ""
    PDFSettings->setValue("UserCreationText", user_creation_text_edit->text());
}

void ConfigurationWidget::setDisableUserText()
{
    // WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "UserDisableText" ""
    PDFSettings->setValue("UserDisableText", user_disable_text_edit->text());
}

void ConfigurationWidget::setImagePath()
{
    // WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "CompanyLogoPath" ""
    PDFSettings->setValue("CompanyLogoPath", image_path_edit->text());
}

void ConfigurationWidget::setLogoPosition(const int &index)
{
    // Default is 'Bottom'
    // Only other option is 'Top'
    // WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "LogoPosition" ""
    if(index == 1)
    {
        PDFSettings->setValue("LogoPosition", "Top");
    }
    else if(index == 2)
    {
        PDFSettings->setValue("LogoPosition", "Bottom");
    }
}

void ConfigurationWidget::setProfileCleanupSettings(const bool &state)
{
    if(state)
    {
        DisableSettings->setValue("CleanupOn", 1);
    }
    else if(!state)
    {
        DisableSettings->setValue("CleanupOn", 0);
    }
}

void ConfigurationWidget::setProfileRedirectionPath()
{
    if(folder_redirection_edit->text().length() > 8 && folder_redirection_edit->text() != "")
    {
        DisableSettings->setValue("RedirectionShare", folder_redirection_edit->text());
    }
    else if(folder_redirection_edit->text().length() < 8 || folder_redirection_edit->text() == "")
    {
        folder_redirection_edit->setText("Either the length of the path is to short or you hit enter while the box was blank. Try again.");
    }
}

void ConfigurationWidget::setProfileStoragePath()
{
    if(profile_storage_edit->text().length() > 8 && profile_storage_edit->text() != "")
    {
        DisableSettings->setValue("StorageLocation", profile_storage_edit->text());
    }
    else if(profile_storage_edit->text().length() < 8 || profile_storage_edit->text() == "")
    {
        profile_storage_edit->setText("Either the length of the path is to short or you hit enter while the box was blank. Try again.");
    }
}

QString ConfigurationWidget::ou_name()
{
    return _oucn;
}

QString ConfigurationWidget::company_name()
{
    return _company;
}

QString ConfigurationWidget::prefix_name()
{
    return _prefix;
}

int ConfigurationWidget::sam_setting()
{
    return _samsetting;
}

//

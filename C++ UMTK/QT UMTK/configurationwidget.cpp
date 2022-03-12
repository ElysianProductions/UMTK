#include "configurationwidget.h"

ConfigurationWidget::ConfigurationWidget()
{
    close_button = new QPushButton();
    user_creation_text_edit = new QLineEdit();
    user_disable_text_edit = new QLineEdit();
    image_path_edit = new QLineEdit();
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

    menus_widget->insertWidget(0, get_company_custimization_widget());
    menus_widget->insertWidget(1, get_pdf_custimization_widget(user_creation_text_edit, user_disable_text_edit, image_path_edit));
    menus_widget->insertWidget(2, get_generation_custimization_widget());

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

QWidget* ConfigurationWidget::get_company_custimization_widget()
{
    QWidget *primary_display = new QWidget();





    return primary_display;
}

QWidget* ConfigurationWidget::get_pdf_custimization_widget(QLineEdit *user_creation_text_edit, QLineEdit *user_disable_text_edit, QLineEdit *image_path_edit)
{
    QGridLayout *primary_layout = new QGridLayout();
    QWidget *primary_display = new QWidget();

    QLabel *new_user_label = new QLabel("New user text:");
    QLabel *disable_user_label = new QLabel("Disable user text:");
    QLabel *image_path_label = new QLabel("Image path:");

    user_creation_text_edit->setPlaceholderText("Enter text");
    user_creation_text_edit->setToolTip("Enter the text to be printed at the top of the new user PDF generated when a user has been created.");

    user_disable_text_edit->setPlaceholderText("Enter text");
    user_disable_text_edit->setToolTip("Enter the text to be printed on the disable user PDF generated when a user has been disabled.");

    image_path_edit->setPlaceholderText("Enter path");
    image_path_edit->setToolTip("Enter the full path to the image yo uwant to be displayed at the bottom of the new user and disabled user PDFs.");

    primary_layout->addWidget(new_user_label, 0, 0);
    primary_layout->addWidget(user_creation_text_edit, 0, 1);
    primary_layout->addWidget(disable_user_label, 1, 0);
    primary_layout->addWidget(user_disable_text_edit, 1, 1);
    primary_layout->addWidget(image_path_label, 2, 0);
    primary_layout->addWidget(image_path_edit, 2, 1);

    primary_display->setLayout(primary_layout);
    return primary_display;
}


QWidget* ConfigurationWidget::get_generation_custimization_widget()
{
    QWidget *primary_display = new QWidget();
    QGridLayout *primary_layout = new QGridLayout();

    // SamAccountSettings

    QCheckBox *sam_account_one = new QCheckBox(); // First initial last name
    QCheckBox *sam_account_two = new QCheckBox(); // Last name first initial
    QCheckBox *sam_account_three = new QCheckBox(); // First name
    QCheckBox *sam_account_four = new QCheckBox(); // Last name
    QCheckBox *sam_account_five = new QCheckBox(); // First name last name
    QCheckBox *sam_account_six = new QCheckBox(); // Last name First name
    QSpacerItem *spacer_one = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    sam_account_one->setText("First initial last name");
    sam_account_two->setText("last name first initial");
    sam_account_three->setText("first name");
    sam_account_four->setText("last name");
    sam_account_five->setText("first name last name");
    sam_account_six->setText("last name first name");
    QLabel *sam_label = new QLabel("SamAccount Settings");
    sam_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    sam_label->setStyleSheet("background-color:white");
    sam_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);


    QButtonGroup *sam_button_group = new QButtonGroup();
    sam_button_group->addButton(sam_account_one);
    sam_button_group->addButton(sam_account_two);
    sam_button_group->addButton(sam_account_three);
    sam_button_group->addButton(sam_account_four);
    sam_button_group->addButton(sam_account_five);
    sam_button_group->addButton(sam_account_six);

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

    QCheckBox *ticketing_top = new QCheckBox();
    QCheckBox *ticketing_bottom = new QCheckBox();
    QButtonGroup *ticket_layout_group = new QButtonGroup();
    ticketing_top->setText("Add ticket to top of page");
    ticketing_bottom->setText("Add ticket to bottom of page");
    ticket_layout_group->addButton(ticketing_top);
    ticket_layout_group->addButton(ticketing_bottom);

    //

    primary_layout->setSpacing(0);
    primary_layout->setHorizontalSpacing(0);
    primary_layout->setVerticalSpacing(0);
    primary_layout->addWidget(sam_label, 0, 0);
    primary_layout->addWidget(sam_account_one, 1, 0);
    primary_layout->addWidget(sam_account_two, 1, 1);
    primary_layout->addWidget(sam_account_three, 1, 2);
    primary_layout->addWidget(sam_account_four, 2, 0);
    primary_layout->addWidget(sam_account_five, 2, 1);
    primary_layout->addWidget(sam_account_six, 2, 2);
    primary_layout->addItem(spacer_one , 3, 0);
    primary_layout->addWidget(ticketing_label, 4, 0);
    primary_layout->addWidget(ticketing_button_on, 5, 0);
    primary_layout->addWidget(ticketing_button_off, 5, 1);
    primary_layout->addWidget(ticketing_top, 6, 0);
    primary_layout->addWidget(ticketing_bottom, 6, 1);
    primary_layout->addItem(spacer_two, 7, 0);



    primary_display->setLayout(primary_layout);

    return primary_display;
}


void ConfigurationWidget::swap_menu(QListWidgetItem *selected, QListWidgetItem *previous)
{
  if(!selected)
  {
      selected = previous;
  }
  menus_widget->setCurrentIndex(menu_options_widget->row(selected));
}



//

#include "stdafx.h"
#include "proxysettingspage.h"

ProxySettingsPage::ProxySettingsPage(QWidget *parnet) : m_parent(parnet)
{
    QGroupBox *updateGroup = new QGroupBox(tr("Package selection"));
    QCheckBox *systemCheckBox = new QCheckBox(tr("Update system"));
    QCheckBox *appsCheckBox = new QCheckBox(tr("Update applications"));
    QCheckBox *docsCheckBox = new QCheckBox(tr("Update documentation"));
    QCheckBox *systemCheckBox2 = new QCheckBox(tr("Update system"));
    QCheckBox *appsCheckBox2 = new QCheckBox(tr("Update applications"));
    QCheckBox *docsCheckBox2 = new QCheckBox(tr("Update documentation"));
    QCheckBox *systemCheckBox3 = new QCheckBox(tr("Update system"));
    QCheckBox *appsCheckBox3 = new QCheckBox(tr("Update applications"));
    QCheckBox *docsCheckBox3 = new QCheckBox(tr("Update documentation"));

    QGroupBox *packageGroup = new QGroupBox(tr("Existing packages"));

    QListWidget *packageList = new QListWidget;
    QListWidgetItem *qtItem = new QListWidgetItem(packageList);
    qtItem->setText(tr("Qt"));
    QListWidgetItem *qsaItem = new QListWidgetItem(packageList);
    qsaItem->setText(tr("QSA"));
    QListWidgetItem *teamBuilderItem = new QListWidgetItem(packageList);
    teamBuilderItem->setText(tr("Teambuilder"));

    QPushButton *startUpdateButton = new QPushButton(tr("Start update"));

    QVBoxLayout *updateLayout = new QVBoxLayout;
    updateLayout->addWidget(systemCheckBox);
    updateLayout->addWidget(appsCheckBox);
    updateLayout->addWidget(docsCheckBox);
    updateLayout->addWidget(systemCheckBox2);
    updateLayout->addWidget(appsCheckBox2);
    updateLayout->addWidget(docsCheckBox2);
    updateLayout->addWidget(systemCheckBox3);
    updateLayout->addWidget(appsCheckBox3);
    updateLayout->addWidget(docsCheckBox3);
    updateGroup->setLayout(updateLayout);

    QVBoxLayout *packageLayout = new QVBoxLayout;
    packageLayout->addWidget(packageList);
    packageGroup->setLayout(packageLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(updateGroup);
    mainLayout->addWidget(packageGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(startUpdateButton);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

ProxySettingsPage::~ProxySettingsPage()
{
}

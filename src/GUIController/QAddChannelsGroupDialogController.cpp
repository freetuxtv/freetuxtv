//
// Created by ebeuque on 10/10/2020.
//

#include <QDir>
#include <QPushButton>
#include <QTreeView>
#include <QLineEdit>
#include <QMessageBox>
#include <QHeaderView>
#include <QXmlInputSource>

#include "Global/QError.h"
#include "Global/FileUtils.h"
#include "Global/UserPaths.h"
#include "Global/Application.h"

#include "Model/Preferences.h"
#include "XML/ChannelsGroupsParserHandler.h"

#include "GUI/QAddChannelsGroupDialog.h"

#include "GUIModel/QChannelsGroupItem.h"
#include "GUIModel/QChannelsGroupListModel.h"

#include "QAddChannelsGroupDialogController.h"

void QAddChannelsGroupDialogController::init(QAddChannelsGroupDialog* pAddChannelsGroupDialog, Application* pApplication)
{
	m_pAddChannelsGroupDialog = pAddChannelsGroupDialog;
	m_pApplication = pApplication;

	// Create model
	m_pChannelsGroupListModel = new QChannelsGroupListModel();

	// Attach model to view
	QTreeView* pTreeView = m_pAddChannelsGroupDialog->getTreeViewChannelsGroupList();
	pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	pTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	pTreeView->setModel(m_pChannelsGroupListModel);
	QHeaderView* pTreeHeader;
	pTreeHeader = pTreeView->header();
	if(pTreeHeader) {
		pTreeHeader->setDefaultAlignment(Qt::AlignCenter);
		//pTreeHeader->setStretchLastSection(true);
		pTreeHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	}

	connect(m_pAddChannelsGroupDialog->getButtonCancel(), SIGNAL(clicked()), m_pAddChannelsGroupDialog, SLOT(reject()));
	connect(m_pAddChannelsGroupDialog->getButtonValid(), SIGNAL(clicked()), this, SLOT(onValidButtonClicked()));

	connect(m_pAddChannelsGroupDialog->getButtonChannelsGroupListRefresh(), SIGNAL(clicked()), this, SLOT(onChannelsGroupListRefreshButtonClicked()));

	// Load channels groups list
	QError error;
	QDir dirPath = UserPaths::getUserCacheDir();
	QString szDstFilePath = dirPath.filePath("channels_groups.dat");
	loadChannelsGroupListFromFile(szDstFilePath, m_pChannelsGroupListModel, &error);
}

void QAddChannelsGroupDialogController::dispose()
{

}

void QAddChannelsGroupDialogController::onValidButtonClicked()
{
	qDebug("onValidButtonClicked");
/*
	QError error;

	GtkWindow* pWindow;
	GtkWidget* widget;
	GtkProgressDialog* pProgressDialog = NULL;

	const gchar *sgroupname = NULL;
	const gchar *sgroupuri = NULL;
	const gchar *sbregex = NULL;
	const gchar *seregex = NULL;

	gboolean has_process = FALSE;

	gchar *tmptext;

	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	widget = (GtkWidget *) gtk_builder_get_object (builder,
												   "notebook_dialogaddgroup");

	pWindow = gtk_builder_window_get_top_window (GTK_BUILDER_WINDOW(pWindowAddChannelsGroup));

	if(error == NULL){
		int page;
		page = gtk_notebook_get_current_page (GTK_NOTEBOOK(widget));
		switch(page){
			case 0:
				// Add one or many groups from the list of existing group
				widget = (GtkWidget *)gtk_builder_get_object (builder,
															  "treeview_channelsgroups");
				GtkTreeSelection *selection;
				selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));

				GtkTreeModel* model;
				model = (GtkTreeModel *) gtk_builder_get_object (builder,
																 "treestore_channelsgroup");
				GList *list;
				list = gtk_tree_selection_get_selected_rows (selection, &model);

				int count;
				int i = 0;
				int nb_added = 0;
				count = gtk_tree_selection_count_selected_rows(selection);

				GList* iterator = NULL;
				iterator = g_list_first (list);

				if(iterator != NULL){
					has_process = TRUE;
				}else{
					errmsg = g_strdup_printf(_("Please select a least one group !"));
				}

				// Initialize the dialog progress
				if(has_process){
					pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(pWindow));
					gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
					gtk_widget_show(GTK_WIDGET(pProgressDialog));
				}

				GtkTreePath* last_ppath = NULL;
				FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;

				while(iterator != NULL && error == NULL){
					GtkTreePath* path;
					GtkTreePath* parent_path;

					GtkTreeIter iter;
					GtkTreeIter iter_parent;
					gboolean valid = TRUE;

					// Get the next path
					path = (GtkTreePath*)iterator->data;

					i++;

					if(gtk_tree_path_get_depth (path) == 1){
						last_ppath = path;

						if(gtk_tree_model_get_iter (model, &iter_parent, path)){
							if(gtk_tree_model_iter_children (model, &iter, &iter_parent)){

								do {
									gtk_tree_model_get (model, &iter, MODEL_CHANNELSGROUP_NAME, &sgroupname,
														MODEL_CHANNELSGROUP_URI, &sgroupuri,
														MODEL_CHANNELSGROUP_BREGEX, &sbregex,
														MODEL_CHANNELSGROUP_EREGEX, &seregex, -1);

									pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
																							  FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST);
									freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos,
																			(gchar*)sgroupuri);
									freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos,
																			  (gchar*)sbregex, (gchar*)seregex);

									// Update progress dialog
									tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
															  pChannelsGroupInfos->name);
									gtk_progress_dialog_set_text(pProgressDialog, tmptext);
									g_free(tmptext);

									// Add the group in the database
									if(error == NULL){
										channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
																			 db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
									}

									if(error == NULL){
										nb_added++;
									}

								}while (gtk_tree_model_iter_next(model, &iter) && error == NULL);

							}
						}
					}else{
						parent_path = gtk_tree_path_copy (path);
						gtk_tree_path_up(parent_path);

						if(last_ppath != NULL){
							if(gtk_tree_path_compare (parent_path, last_ppath) == 0){
								valid = FALSE;
							}
						}
						gtk_tree_path_free (parent_path);

						if(valid){
							if(gtk_tree_model_get_iter (model, &iter, path)){
								gtk_tree_model_get (model, &iter, MODEL_CHANNELSGROUP_NAME, &sgroupname,
													MODEL_CHANNELSGROUP_URI, &sgroupuri,
													MODEL_CHANNELSGROUP_BREGEX, &sbregex,
													MODEL_CHANNELSGROUP_EREGEX, &seregex, -1);

								pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
																						  FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST);
								freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos,
																		(gchar*)sgroupuri);
								freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, (gchar*)sbregex, (gchar*)seregex);

								// Update progress dialog
								tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
														  pChannelsGroupInfos->name);
								gtk_progress_dialog_set_text(pProgressDialog, tmptext);
								g_free(tmptext);

								// Add the group in the database
								if(error == NULL){
									channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
																		 db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
								}

								if(error == NULL){
									nb_added++;
								}
							}
						}
					}

					// Update the progress bar
					gdouble percent;
					percent = (gdouble)i/(gdouble)count;
					gtk_progress_dialog_set_percent(pProgressDialog, percent);

					iterator = g_list_next(iterator);
				}
				g_list_free (list);

				if(has_process){
					tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), nb_added);
					gtk_progress_dialog_set_text(pProgressDialog, tmptext);
					gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
					gtk_progress_dialog_set_button_close_enabled(pProgressDialog, TRUE);
					g_free(tmptext);
				}

				break;
			case 1:
				// Add custom group
				widget = (GtkWidget *) gtk_builder_get_object (builder,
															   "dialogaddgroup_name");
				sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));

				widget = (GtkWidget *) gtk_builder_get_object (builder,
															   "dialogaddgroup_uri");
				sgroupuri = gtk_entry_get_text(GTK_ENTRY(widget));

				widget = (GtkWidget *) gtk_builder_get_object (builder,
															   "dialogaddgroup_bregex");
				sbregex = gtk_entry_get_text(GTK_ENTRY(widget));

				widget = (GtkWidget *) gtk_builder_get_object (builder,
															   "dialogaddgroup_eregex");
				seregex = gtk_entry_get_text(GTK_ENTRY(widget));

				// Check the field
				if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
					errmsg = g_strdup_printf(_("Please enter the group's name !"));
				}
				if(g_ascii_strcasecmp(sgroupuri,"") == 0 && errmsg == NULL){
					errmsg = g_strdup_printf(_("Please enter the group's URI !"));
				}

				if(errmsg == NULL){
					pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(pWindow));
					gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
					gtk_widget_show(GTK_WIDGET(pProgressDialog));

					if(g_ascii_strcasecmp(sbregex,"") == 0 ){
						sbregex = NULL;
					}
					if(g_ascii_strcasecmp(seregex,"") == 0 ){
						seregex = NULL;
					}

					FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;
					pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
																			  FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST);
					freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos,
															(gchar*)sgroupuri);
					freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, (gchar*)sbregex, (gchar*)seregex);

					// Update progress dialog
					tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
											  pChannelsGroupInfos->name);
					gtk_progress_dialog_set_text(pProgressDialog, tmptext);
					gtk_progress_dialog_set_percent(pProgressDialog, 0.0);
					g_free(tmptext);

					// Add the group in the database
					if(error == NULL){
						channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
															 db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
					}

					if(error == NULL){
						tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), 1);
						gtk_progress_dialog_set_text(pProgressDialog, tmptext);
						gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
						gtk_progress_dialog_set_button_close_enabled(pProgressDialog, TRUE);

						g_free(tmptext);
					}
				}

				break;
			case 2:
				// Add custom group
				widget = (GtkWidget *) gtk_builder_get_object (builder,
															   "dialogaddgroup_specialgroupname");
				sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));

				// Check the field
				if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
					errmsg = g_strdup_printf(_("Please enter the group's name !"));
				}

				if(errmsg == NULL){
					pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(pWindowAddChannelsGroup));
					gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
					gtk_widget_show(GTK_WIDGET(pProgressDialog));

					FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;
					pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
																			  FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES);

					// Update progress dialog
					tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
											  pChannelsGroupInfos->name);
					gtk_progress_dialog_set_text(pProgressDialog, tmptext);
					gtk_progress_dialog_set_percent(pProgressDialog, 0.0);
					g_free(tmptext);

					// Add the group in the database
					if(error == NULL){
						channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
															 db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
					}

					if(error == NULL){
						tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), 1);
						gtk_progress_dialog_set_text(pProgressDialog, tmptext);
						gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
						gtk_progress_dialog_set_button_close_enabled(pProgressDialog, TRUE);

						g_free(tmptext);
					}
				}
				break;
		}
	}
	dbsync_close_db(&dbsync);

	// On error we destroy the progress dialog view
	if(pProgressDialog && (errmsg != NULL || error != NULL)){
		gtk_widget_destroy (GTK_WIDGET(pProgressDialog));
		pProgressDialog = NULL;
	}

	if(errmsg != NULL){
		windowmain_show_error (app, errmsg);
		g_free(errmsg);
	}
	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}*/

};

void QAddChannelsGroupDialogController::onChannelsGroupListRefreshButtonClicked()
{
	bool bGoOn = true;

	QError error;

	qInfo("Starting update of the channel's groups list");

	QString szURI = m_pAddChannelsGroupDialog->getLineEditChannelsGroupFile()->text();

	QDir dirPath = UserPaths::getUserCacheDir();
	QString szDstFilePath = dirPath.filePath("channels_groups.dat");

	// Download the file
	qInfo("Downloading the file '%s'", qPrintable(szURI));
	bGoOn = FileUtils::getFile(szURI, szDstFilePath, &m_pApplication->getPreferences()->m_proxy, m_pApplication->getPreferences()->m_iTimeout, &error);

	if(bGoOn){
		qInfo("Updating the list of channel's groups");
		bGoOn = loadChannelsGroupListFromFile(szDstFilePath, m_pChannelsGroupListModel, &error);
	}

	if(bGoOn) {
		qInfo("The list of channel's groups has been updated");
	}else{
		qCritical("Error when updating the list of channel's groups");
		QMessageBox::critical(m_pAddChannelsGroupDialog, tr("Error"), error.getErrorMsg());
	}
};


bool QAddChannelsGroupDialogController::loadChannelsGroupListFromFile(const QString& szFilePath, QChannelsGroupListModel* pModel, QError *pError)
{
	bool bRes = true;

	QString szTmpFilePath = szFilePath;

	QFile file(szTmpFilePath);
	if(!file.exists()){
		// If not local file, we use the file from
		szTmpFilePath = QDir(m_pApplication->getDataDir()).filePath("channels_groups.xml");
		file.setFileName(szTmpFilePath);
	}

	QList<ChannelsGroupSection> listChannelsGroupSection;

	// Parse the file
	bRes = file.open(QIODevice::ReadOnly);
	if(bRes){
		QXmlInputSource source(&file);

		QXmlSimpleReader reader;
		ChannelsGroupsParserHandler handler(listChannelsGroupSection);
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);

		bRes = reader.parse(source);
		if(!bRes){
			qCritical("[XML] Error to parse the content of file: %s", qPrintable(szTmpFilePath));
		}

		file.close();
	}else{
		qCritical("[XML] Error to open the file: %s", qPrintable(szTmpFilePath));
	}

	// Fill the treeview
	if(bRes){
		// Clear the model
		pModel->clear();
		bRes = fillChannelsGroupModel(listChannelsGroupSection, pModel, pError);
	}

	return bRes;
}

bool QAddChannelsGroupDialogController::fillChannelsGroupModel(const QList<ChannelsGroupSection>& listChannelsGroupSection, QChannelsGroupListModel* pModel, QError *pError)
{
	bool bRes = true;

	QList<ChannelsGroupSection>::const_iterator iter_section;
	QList<ChannelsGroupInfos>::const_iterator iter_group;

	QChannelsGroupItem* pSectionItem;
	QChannelsGroupItem* pChannelsGroupItem;

	for(iter_section = listChannelsGroupSection.constBegin(); iter_section != listChannelsGroupSection.constEnd(); ++iter_section)
	{
		const ChannelsGroupSection& channelsGroupSection = (*iter_section);
		pSectionItem = new QChannelsGroupItem(channelsGroupSection);
		pModel->appendRow(pSectionItem);

		const QList<ChannelsGroupInfos>& listChannelsGroupInfos = channelsGroupSection.getChannelsGroupList();

		for(iter_group = listChannelsGroupInfos.constBegin(); iter_group != listChannelsGroupInfos.constEnd(); ++iter_group)
		{
			const ChannelsGroupInfos& channelsGroupInfos = (*iter_group);
			pChannelsGroupItem = new QChannelsGroupItem(channelsGroupInfos);
			pSectionItem->appendRow(pChannelsGroupItem);
		}
	}

	return bRes;
}
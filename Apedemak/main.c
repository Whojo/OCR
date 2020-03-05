#include "UI/ui.h"

int main() {

    //Initializes GTK
    gtk_init(NULL,NULL);

    //Load .glade for UI
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if(gtk_builder_add_from_file(builder, "apedemak.glade", &error) == 0) {
        g_print("Error while loading ui: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Link Css
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "./style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), 
        GTK_STYLE_PROVIDER(css),GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Get widget
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
    GtkFileChooser* fileChooserButton = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "fileChooserButton"));

    // Image widget
    GtkImage* imageFrame = GTK_IMAGE(gtk_builder_get_object(builder, "displayImage"));

    // Tool button
    GtkButton* grayscaleBtn = GTK_BUTTON(gtk_builder_get_object(builder, "grayscaleButton"));
    GtkButton* blackAndWhiteBtn = GTK_BUTTON(gtk_builder_get_object(builder, "blackandwhiteButton"));
    GtkButton* trainButton = GTK_BUTTON(gtk_builder_get_object(builder, "trainButton"));
    GtkButton* segmentationButton = GTK_BUTTON(gtk_builder_get_object(builder, "segmentationBtn"));
    GtkButton* upperRecoBtn = GTK_BUTTON(gtk_builder_get_object(builder, "upperRecoBtn"));
    GtkButton* lowerRecoBtn = GTK_BUTTON(gtk_builder_get_object(builder, "lowerRecoBtn"));
    GtkButton* digitRecoBtn = GTK_BUTTON(gtk_builder_get_object(builder, "digitRecoBtn"));
    GtkButton* globalRecoBtn = GTK_BUTTON(gtk_builder_get_object(builder, "globalRecoBtn"));

    // Training window
    GtkWindow *trainWindow = GTK_WINDOW(gtk_builder_get_object(builder, "trainingWindow"));
    GtkFileChooser *trainingFileChooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "trainFileChooserBtn"));
    GtkButton *majTrainingBtn = GTK_BUTTON(gtk_builder_get_object(builder, "majTrainingBtn"));
    GtkButton *minTrainingBtn = GTK_BUTTON(gtk_builder_get_object(builder, "minTrainingBtn"));
    GtkButton *numTrainingBtn = GTK_BUTTON(gtk_builder_get_object(builder, "numTrainingBtn"));
    GtkButton *globalTrainingBtn = GTK_BUTTON(gtk_builder_get_object(builder, "globalTrainingBtn"));

    // Message Window
    GtkWindow *mWindow = GTK_WINDOW(gtk_builder_get_object(builder, "messageWindow"));
    GtkLabel *msgLabel = GTK_LABEL(gtk_builder_get_object(builder, "messageLabel"));
    GtkButton *okMessageWindow = GTK_BUTTON(gtk_builder_get_object(builder, "okMessageButton"));
    GtkImage *imgMessageWindow = GTK_IMAGE(gtk_builder_get_object(builder, "imageMessageWindow"));

    //Output
    GtkWindow *outputWindow = GTK_WINDOW(gtk_builder_get_object(builder, "outputWindow"));
    GtkTextView *outputTextView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "outputTextView"));
    GtkButton *closeOutputBtn = GTK_BUTTON(gtk_builder_get_object(builder, "closeOutputWindowBtn"));
    GtkButton *spellCheckingBtn = GTK_BUTTON(gtk_builder_get_object(builder, "correctBtn"));

    //Filter for file chooser
    GtkFileFilter *filterImg = gtk_file_filter_new();
    gtk_file_filter_set_name(filterImg, "Toutes les images");
    gtk_file_filter_add_pattern(filterImg, "*.bmp");
    gtk_file_filter_add_pattern(filterImg, "*.png");
    gtk_file_filter_add_pattern(filterImg, "*.jpeg");
    gtk_file_filter_add_pattern(filterImg, "*.jpg");
    gtk_file_filter_add_pattern(filterImg, "*.gif");

    GtkFileFilter *filterAll = gtk_file_filter_new();
    gtk_file_filter_set_name(filterAll, "Tout");
    gtk_file_filter_add_pattern(filterAll, "*");

    gtk_file_chooser_add_filter(fileChooserButton, filterImg);
    gtk_file_chooser_add_filter(fileChooserButton, filterAll);

    // Initailizes data for the program
    data data =  { 
        .image = {
            .path = "./data/logo.jpg",
            .grayPath = "",
            .blackPath = "",
            .segmentedPath = "",
        },
        .msgWindow = 
        {
            .gtkWindow = mWindow,
            .label = msgLabel,
            .okButton = okMessageWindow,
            .imageMessageWindow = imgMessageWindow,
        },
        .trainingWindow = 
        {
            .window = trainWindow,
            .fileChooser = trainingFileChooser,
        },
        .output = 
        {
            .window = outputWindow,
            .gtkTextView = outputTextView,
            .buffer = gtk_text_view_get_buffer(outputTextView),
            .outputFile = "",
        },
        .mainFileChooser = fileChooserButton,
        .display = imageFrame,
        .saveFolder = "./data/",
    };

    // Connect signal
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(fileChooserButton, "selection-changed", G_CALLBACK(setImage), &data);

    // Main button signal
    g_signal_connect(grayscaleBtn, "clicked", G_CALLBACK(on_clicked_grayscale), &data);
    g_signal_connect(blackAndWhiteBtn, "clicked", G_CALLBACK(on_clicked_blackwhite), &data);
    g_signal_connect(trainButton, "clicked", G_CALLBACK(open_training_window), &data);

    // Recognition signal
    g_signal_connect(upperRecoBtn, "clicked", G_CALLBACK(on_upper_reco), &data);
    g_signal_connect(lowerRecoBtn, "clicked", G_CALLBACK(on_lower_reco), &data);
    g_signal_connect(digitRecoBtn, "clicked", G_CALLBACK(on_digit_reco), &data);
    g_signal_connect(globalRecoBtn, "clicked", G_CALLBACK(on_global_reco), &data);

    //Training window
    g_signal_connect(GTK_WIDGET(trainWindow), "delete-event", G_CALLBACK(hide_widget), NULL);
    g_signal_connect(majTrainingBtn, "clicked", G_CALLBACK(maj_training), &data);
    g_signal_connect(minTrainingBtn, "clicked", G_CALLBACK(min_training), &data);
    g_signal_connect(numTrainingBtn, "clicked", G_CALLBACK(num_training), &data);
    g_signal_connect(globalTrainingBtn, "clicked", G_CALLBACK(global_training), &data);

    //Message Window 
    g_signal_connect(mWindow, "delete-event", G_CALLBACK(hide_widget), NULL);
    g_signal_connect(okMessageWindow, "clicked", G_CALLBACK(closeMessageWindow), &data);
    
    //Output window
    g_signal_connect(outputWindow, "delete-event", G_CALLBACK(hide_widget), NULL);
    g_signal_connect(segmentationButton, "clicked", G_CALLBACK(on_segmentation_clicked), &data);
    g_signal_connect(closeOutputBtn, "clicked", G_CALLBACK(close_output), &data);  
    g_signal_connect(spellCheckingBtn, "clicked", G_CALLBACK(spellchecking), &data);

    gtk_main();


    return 0;
}

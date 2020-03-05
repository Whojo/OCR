#include "ui.h"

#define DEFAULT_IMAGE "./data/logo.jpg"
#define NO_FILE_SELECTED "No file selected!"
#define LOAD_FAILED "Unable to load image!"
#define ERROR_ICON "gtk-dialog-warning"
#define VALID_ICON "gtk-ok"

gboolean hide_widget(GtkWidget *widget) {
    gtk_widget_hide(widget);
    return 1;
}

void displayImage(GtkImage* imageFrame, char *filepath) {
    gtk_image_set_from_file(imageFrame, filepath);
}

void showMessage(messageWindow window, char *type, char *message) {
    if(!gtk_widget_is_visible(GTK_WIDGET(window.gtkWindow))) {
        gtk_widget_show(GTK_WIDGET(window.gtkWindow));
        gtk_image_set_from_icon_name(window.imageMessageWindow, type,
             GTK_ICON_SIZE_DIALOG);
        gtk_label_set_text(window.label, message);
    }
}

gboolean closeMessageWindow(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    hide_widget(GTK_WIDGET(data->msgWindow.gtkWindow));
    return 1;
}

gboolean close_output(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    hide_widget(GTK_WIDGET(data->output.window));
    return 1;
}

gboolean setImage(GtkFileChooser* fileChooser, gpointer user_data) {
    data *data = user_data;
    char *imagePath = gtk_file_chooser_get_filename(fileChooser);
    if(imagePath != NULL && imagePath != (char *)"") {
        data->image.path = imagePath;
        displayImage(data->display, data->image.path);
    }
    return 1;
}

gboolean on_clicked_grayscale(GtkButton* button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // check if image has been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            displayImage(data->display, data->image.grayPath);
        }
    }
    return 1;
}

gboolean on_clicked_blackwhite(GtkButton* button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    // If our image path is null or empty
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // If image has not been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            img = load_image(data->image.grayPath);
            Black_And_White(img);
            data->image.blackPath = "./data/black.bmp";
            img = load_image(data->image.blackPath);
            displayImage(data->display, data->image.blackPath);
        }
    }
    return 1;
}

gboolean open_training_window(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    gtk_widget_show(GTK_WIDGET(data->trainingWindow.window));
    return 1;
}

gboolean on_segmentation_clicked(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // If image has not been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            img = load_image(data->image.grayPath);
            Black_And_White(img);
            data->image.blackPath = "./data/black.bmp";
            img = load_image(data->image.blackPath);
            Segmentation_G(img);
            data->image.segmentedPath = "./data/result.bmp";
            displayImage(data->display, data->image.segmentedPath);
        }
    }
    return 1;
}
gboolean maj_training(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    char *file = gtk_file_chooser_get_filename(data->trainingWindow.fileChooser);
    if(file == NULL)
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        TrainNeuralNetworkLettersMaj(file);
    }
    return 1;
}

gboolean min_training(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    char *file = gtk_file_chooser_get_filename(data->trainingWindow.fileChooser);
    if(file == NULL)
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        TrainNeuralNetworkLettersMin(file);
    }
    return 1;
}

gboolean num_training(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    char *file = gtk_file_chooser_get_filename(data->trainingWindow.fileChooser);
    if(file == NULL)
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        TrainNeuralNetworkDigit(file);
    }
    return 1;
}

gboolean global_training(GtkButton *button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    char *file = gtk_file_chooser_get_filename(data->trainingWindow.fileChooser);
    if(file == NULL)
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        TrainNeuralNetworkApedemak(file);
    }
    return 1;
}

gboolean on_upper_reco(GtkButton* button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // If image has not been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            img = load_image(data->image.grayPath);
            Black_And_White(img);
            data->image.blackPath = "./data/black.bmp";
            img = load_image(data->image.blackPath);
            struct par *segmentationRes = Segmentation_G(img);
            data->image.segmentedPath = "./data/result.bmp";
            ApedemakMain(segmentationRes, "./data/output.txt", 
                "./NeuralNetwork/Data/ThetaLettMaj", '2');
            gtk_widget_show(GTK_WIDGET(data->output.window));
            displayImage(data->display, data->image.segmentedPath);
            printFile("./data/output.txt", data);
        }
    }
    return 1;
}

gboolean on_lower_reco(GtkButton* button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // If image has not been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            img = load_image(data->image.grayPath);
            Black_And_White(img);
            data->image.blackPath = "./data/black.bmp";
            img = load_image(data->image.blackPath);
            struct par *segmentationRes = Segmentation_G(img);
            data->image.segmentedPath = "./data/result.bmp";
            ApedemakMain(segmentationRes, "./data/output.txt",
                 "./NeuralNetwork/Data/ThetaLettMin", '1');
            gtk_widget_show(GTK_WIDGET(data->output.window));
            displayImage(data->display, data->image.segmentedPath);
            printFile("./data/output.txt", data);
        }
    }
    return 1;
}
gboolean on_digit_reco(GtkButton* button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // If image has not been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            img = load_image(data->image.grayPath);
            Black_And_White(img);
            data->image.blackPath = "./data/black.bmp";
            img = load_image(data->image.blackPath);
            struct par *segmentationRes = Segmentation_G(img);
            data->image.segmentedPath = "./data/result.bmp";
            ApedemakMain(segmentationRes, "./data/output.txt",
                 "./NeuralNetwork/Data/ThetaDigit", '0');
            gtk_widget_show(GTK_WIDGET(data->output.window));
            displayImage(data->display, data->image.segmentedPath);
            printFile("./data/output.txt", data);
        }
    }
    return 1;
}
gboolean on_global_reco(GtkButton* button, gpointer user_data) {
    data *data = user_data;
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    if(data->image.path == (char*)"" || data->image.path == NULL) 
        showMessage(data->msgWindow, ERROR_ICON, NO_FILE_SELECTED);
    else {
        SDL_Surface* img = load_image(data->image.path);
        // If image has not been load
        if(img == NULL) {
            data->image.path = "";
            showMessage(data->msgWindow, ERROR_ICON, LOAD_FAILED);
            gtk_file_chooser_unselect_all(data->mainFileChooser);
        }
        else {
            grayscale(img);
            data->image.grayPath = "./data/gray.bmp";
            img = load_image(data->image.grayPath);
            Black_And_White(img);
            data->image.blackPath = "./data/black.bmp";
            img = load_image(data->image.blackPath);
            struct par *segmentationRes = Segmentation_G(img);
            data->image.segmentedPath = "./data/result.bmp";
            ApedemakMain(segmentationRes, "./data/output.txt", 
                "./NeuralNetwork/Data/ThetaApedemak", '3');
            gtk_widget_show(GTK_WIDGET(data->output.window));
            displayImage(data->display, data->image.segmentedPath);
            printFile("./data/output.txt", data);
        }
    }
    return 1;
}
void printFile(char* filepath, data *data) {
    FILE *toPrint = fopen(filepath ,"r");
    data->output.buffer = gtk_text_buffer_new(NULL);
    gtk_text_view_set_buffer(data->output.gtkTextView, data->output.buffer);
    char c = fgetc(toPrint);
    while(c != EOF) {
        insertTextInView(data->output.buffer, &c);
        c = fgetc(toPrint);
    }
    fclose(toPrint);
}

void insertTextInView(GtkTextBuffer *textBuffer, char *str) {
    gtk_text_buffer_insert_at_cursor(textBuffer, str, -1);
}

gboolean spellchecking(GtkButton *button, gpointer user_data) {
    gtk_widget_set_focus_on_click(GTK_WIDGET(button), TRUE);
    data *data = user_data;
    correct();
    printFile("./data/newOutput.txt", data);
    return 1;
}

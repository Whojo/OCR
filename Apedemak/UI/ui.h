#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <math.h>
#include "../NeuralNetwork/NeuralNetworks.h"
#include "../SDL/sdl.h"
#include "../Segmentation/segmentation_G.h"
#include "../Correction/CorrectAI.h"

typedef struct ImageDic {
    char *path;
    char *grayPath;
    char *blackPath;
    char *segmentedPath;
} ImageDic;

typedef struct textView {
    GtkWindow *window;
    GtkTextView *gtkTextView;
    GtkTextBuffer *buffer;
    char* outputFile;
} textView;

typedef struct messageWindow {
    GtkWindow *gtkWindow;
    GtkImage *imageMessageWindow;
    GtkLabel *label;
    GtkButton *okButton;
} messageWindow;

typedef struct TrainingWindow {
    GtkWindow *window;
    GtkFileChooser *fileChooser;
} TrainingWindow;

typedef struct data {
    ImageDic image;
    messageWindow msgWindow;
    TrainingWindow trainingWindow;
    textView output;
    GtkWindow *resultWindow;
    GtkImage *imageWindowFrame;
    GtkImage *display;
    GtkFileChooser *mainFileChooser;
    char *saveFolder;
} data;

gboolean hide_widget(GtkWidget *widget);
void displayImage(GtkImage* imageFrame, char *filepath);
void showMessage(messageWindow window, char *type, char *message);
gboolean closeMessageWindow(GtkButton *button, gpointer user_data);
gboolean close_output(GtkButton *button, gpointer user_data);
gboolean setImage(GtkFileChooser* fileChooser, gpointer user_data);
gboolean on_clicked_grayscale(GtkButton* button, gpointer user_data);
gboolean on_clicked_blackwhite(GtkButton* button, gpointer user_data);
gboolean open_training_window(GtkButton *button, gpointer user_data);
gboolean maj_training(GtkButton *button, gpointer user_data);
gboolean min_training(GtkButton *button, gpointer user_data);
gboolean num_training(GtkButton *button, gpointer user_data);
gboolean global_training(GtkButton *button, gpointer user_data);
gboolean on_segmentation_clicked(GtkButton* button, gpointer user_data);
gboolean on_upper_reco(GtkButton* button, gpointer user_data);
gboolean on_lower_reco(GtkButton* button, gpointer user_data);
gboolean on_digit_reco(GtkButton* button, gpointer user_data);
gboolean on_global_reco(GtkButton* button, gpointer user_data);
gboolean spellchecking(GtkButton* button, gpointer user_data);
void insertTextInView(GtkTextBuffer *textBuffer, char *str);
void printFile(char* filepath, data *data);

#endif
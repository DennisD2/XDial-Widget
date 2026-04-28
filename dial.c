/*
 *  Based on code from 1989 by Douglas A. Young
 *  See chapter 15 of his book, Dial.c code
 */

/*************************************************************
 * dial.c : test the Dial widget class
 *************************************************************/

#include <X11/Intrinsic.h>
#include "Dial.h"

void select_callback();

int main(int argc, char **argv) {
  Widget toplevel, dial;
  /*
   * Initialize the Intrinsics.
   */
  toplevel = XtInitialize(argv[0], "DialTest", NULL,
                          0, &argc, argv);
  /*
   * Create a dial widget and add a select callback.
   */
  dial = XtCreateManagedWidget("dial", XsdialWidgetClass,
                               toplevel, NULL, 0);
  XtAddCallback(dial, XtNselect, select_callback, NULL);
  XtRealizeWidget(toplevel);
  XtMainLoop();
}

void select_callback(Widget w, caddr_t client_data,int position) {
  Arg wargs[1];

  XtSetArg(wargs[0], XtNposition, position);
  XtSetValues(w, wargs, 1);
}

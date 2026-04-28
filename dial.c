/*
*                Copyright 1989 by Douglas A. Young
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation. The author disclaims all warranties with
 * regard to this software, including all implied warranties of
 * merchantability and fitness.
 *
 * Comments and additions may be sent the author at:
 *
 *  dayoung@hplabs.hp.com
 */

/*************************************************************
 * dial.c : test the Dial widget class
 *************************************************************/

#include <X11/Intrinsic.h>
#include "Dial.h"

void select_callback();

void main(argc, argv)
    int   argc;
char *argv[];
{
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

void select_callback(w, client_data,position)
     Widget     w;
caddr_t    client_data;
int        position;
{
  Arg wargs[1];

  XtSetArg(wargs[0], XtNposition, position);
  XtSetValues(w, wargs, 1);
}

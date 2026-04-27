/*****************************************************
 * DialP.h: Private header file for Dial Widget Class
 *****************************************************/


#ifndef DIALP_H
#define DIALP_H

#define MAXSEGMENTS 200

typedef struct _XsDialClassPart{
       int ignore;
} XsDialClassPart;

typedef struct _XsDialClassRec{
   CoreClassPart         core_class;
   XsDialClassPart       dial_class;
} XsDialClassRec;

extern XsDialClassRec XsdialClassRec;

typedef struct _XsDialPart {
   int 	     indicator_type;   /* Type of indicator 	 */ 
   Pixel     indicator_color;  /* Color of the indicator */
   Dimension indicator_thickness;
   int       marker_type;      /* Type of marker 	 */
   Pixel     foreground;       /* Color of the markers   */
   Dimension marker_thickness;

   Boolean   show_midpoint;    /* show middle of dial    */
   Dimension midpoint_thickness;
   Boolean   show_markervalues;/* draw values of markers */
   int       border_type;      /* Type of borde          */
   char*     title;	       /* Title of dial          */
   XFontStruct *font;	       /* Font used for title    */
   Boolean   show_value;       /* Draw current int value */

   int       minimum;          /* minimum value          */
   int       maximum;          /* maximum value          */
   int       markers;          /* number of marks        */
   Dimension marker_length;    /* in pixels              */
   Dimension marker_highlight; /* all nth markers a 	 */
			       /*     highlighted one    */
   Position  position;         /* indicator position     */
   Position  indicator_x;      /* x,y position of tip    */
   Position  indicator_y;      /*     of the indicator   */
   Position  center_x;         /* coords of the          */
   Position  center_y;         /*     dial center        */
   GC        dial_GC;          /* assorted gc's          */
   GC        indicator_GC;
   GC        inverse_GC;
   XPoint    segments[MAXSEGMENTS];
   XtCallbackList select;    /* callback list          */
   XtCallbackList release;    /* callback list          */
} XsDialPart;

typedef struct _XsDialRec {
   CorePart          core;
   XsDialPart        dial;
} XsDialRec;

#define INDICATOR_LINE 0
#define INDICATOR_ARROW 1
#define MARKER_LINE 0
#define MARKER_POINT 1
#define BORDER_NONE 0
#define BORDER_CIRCLE 1
#define BORDER_RECTANGLE 2

#endif /*DIALP_H*/


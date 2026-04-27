/*****************************************************
 * Dial.h: Public header file for Dial Widget Class
 *****************************************************/
#ifndef  DIAL_H
#define  DIAL_H

#include <X11/StringDefs.h>

extern WidgetClass XsdialWidgetClass;
typedef struct _XsDialClassRec * XsDialWidgetClass;
typedef struct _XsDialRec      * XsDialWidget;
/*
 * Define resource strings for the Dial widget.
 */
#define XtNselect         	"select"
#define XtNmarkers        	"markers"
#define XtNminimum        	"minimum"
#define XtNmaximum        	"maximum"
#define XtNindicatorColor 	"indicatorColor"
#define XtNposition       	"position"
#define XtNmarkerLength   	"markerLength"

#define XtCMarkers        	"Markers"
#define XtCMin            	"Min"
#define XtCMax            	"Max"

#define XtNrelease		"release"
#define XtCIndicatorType	"IndicatorType"
#define XtCMarkerType		"MarkerType"
#define XtCBorderType		"BorderType"

#define XtRIndicatorType	"IndicatorType"
#define XtRMarkerType		"MarkerType"
#define XtRBorderType		"BorderType"

#define XtNindicatorType  	"indicatorType"
#define XtNindicatorThickness 	"indicatorThickness"
#define XtNmarkerType	  	"markerType"
#define XtNmarkerThickness 	"markerThickness"
#define XtNmarkerColor	  	"markerColor"
#define XtNshowMid	  	"showMid"
#define XtNmidThickness		"midThickness"
#define XtNshowMarkerValues 	"showMarkerValues"
#define XtNborderType	   	"borderType"
#define XtNtitle		"title"
#define XtNshowValue		"showValue"
#define XtNmarkerHighlight	"markerHighlight"

#define Xt_INDICATOR_LINE	"line"
#define Xt_INDICATOR_ARROW	"arrow"
#define Xt_MARKER_LINE		"line"
#define Xt_MARKER_POINT		"point"
#define Xt_BORDER_NONE		"none"
#define Xt_BORDER_CIRCLE	"circle"
#define Xt_BORDER_RECTANGLE	"rectangle"

extern Widget XtCreateDial( Widget parent, char *name, ArgList args,
	int ArgCount );

#endif DIAL_H

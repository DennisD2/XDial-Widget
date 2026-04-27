/*
 *  Based on code from 1989 by Douglas A. Young
 *  See chapter 12 of his book, Dial.c code
 *  dayoung@hplabs.hp.com
 */

/********************************************************
 * Dial.c: The Dial Widget Methods
 *********************************************************/
#include <stdio.h>
#include <math.h>
#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>
#include "DialP.h"
#include "Dial.h"

#define  RADIANS(x)  (M_PI * 2.0 * (x) / 360.0)
#define  DEGREES(x)  ((x) / (M_PI * 2.0) * 360.0)
#define  MIN_ANGLE   225.0
#define  MAX_ANGLE   270.0

static void    select_dial ();
static void    drag_dial();

static void    Initialize();
static void    Redisplay();
static void    Resize();
static void    Destroy();
static Boolean SetValues();
static void ClassInitialize();

static void calculate_indicator_pos();
static void draw_indicator( XsDialWidget w);

static char defaultTranslations[] =  
	"<Btn1Down>: select()\n\
	 <Btn2Motion>: drag() ";

static XtActionsRec actionsList[] = {
  { "select",   (XtActionProc) select_dial},
  { "drag",  (XtActionProc) drag_dial}
};

static XtResource resources[] = {
  {XtNmarkers, XtCMarkers, XtRInt, sizeof (int),
    XtOffset(XsDialWidget, dial.markers), XtRString, "10" },
  {XtNminimum, XtCMin, XtRInt, sizeof (int),
    XtOffset(XsDialWidget, dial.minimum), XtRString, "0"  },
  {XtNmaximum, XtCMax, XtRInt, sizeof (int),
    XtOffset(XsDialWidget, dial.maximum), XtRString, "100"},
  {XtNindicatorColor, XtCColor, XtRPixel, sizeof (Pixel),
    XtOffset(XsDialWidget, dial.indicator_color), 
    XtRString, "Black"                                    },
  {XtNposition, XtCPosition, XtRPosition, sizeof (Position),
   XtOffset(XsDialWidget, dial.position), XtRString, "0"  },
  {XtNmarkerLength, XtCLength, XtRDimension, sizeof (Dimension),
    XtOffset(XsDialWidget, dial.marker_length),
    XtRString, "5"                                        },
  {XtNforeground, XtCForeground, XtRPixel, sizeof (Pixel),
    XtOffset(XsDialWidget, dial.foreground), 
    XtRString, "Black"                                    },
  {XtNselect, XtCCallback, XtRCallback, sizeof(caddr_t),
    XtOffset (XsDialWidget, dial.select), 
    XtRCallback, NULL                                     },

  {XtNindicatorType, XtCIndicatorType, XtRIndicatorType, sizeof (int),
    XtOffset(XsDialWidget, dial.indicator_type), 
    XtRString, Xt_INDICATOR_LINE			  },
  {XtNindicatorThickness, XtCThickness, XtRDimension, sizeof (Dimension),
    XtOffset(XsDialWidget, dial.indicator_thickness),
    XtRString, "1"					  },
  {XtNmarkerType, XtCMarkerType, XtRMarkerType, sizeof (int),
    XtOffset(XsDialWidget, dial.marker_type), 
    XtRString, Xt_MARKER_LINE			  	  }, 
  {XtNmarkerThickness, XtCThickness, XtRDimension, sizeof (Dimension),
    XtOffset(XsDialWidget, dial.marker_thickness),
    XtRString, "1"                                        },
  {XtNmarkerHighlight, XtCMin, XtRDimension, sizeof (Dimension),
    XtOffset(XsDialWidget, dial.marker_highlight),
    XtRString, "0"                                        },
  {XtNshowMid, XtCBoolean, XtRBoolean, sizeof(Boolean),
    XtOffset(XsDialWidget, dial.show_midpoint),
    XtRString, "False"					  },
  {XtNmidThickness, XtCThickness, XtRDimension, sizeof (Dimension),
    XtOffset(XsDialWidget, dial.midpoint_thickness),
    XtRString, "2"					  },
  {XtNshowMarkerValues, XtCBoolean, XtRBoolean, sizeof(Boolean),
    XtOffset(XsDialWidget, dial.show_markervalues),
    XtRString, "False"                                   },
  {XtNborderType, XtCBorderType, XtRBorderType, sizeof (int),
    XtOffset(XsDialWidget, dial.border_type), 
    XtRString, Xt_BORDER_NONE                          	  },
  {XtNtitle, XtCString, XtRString, sizeof(char *),
    XtOffset(XsDialWidget, dial.title),
    XtRString, ""					  },
  {XtNfont, XtCFont, XtRFontStruct, sizeof (XFontStruct *),
     XtOffset(XsDialWidget, dial.font), 
     XtRString, "Fixed"    				  },
  {XtNshowValue, XtCBoolean, XtRBoolean, sizeof(Boolean),
    XtOffset(XsDialWidget, dial.show_value),
    XtRString, "False"                                   }
 };

XsDialClassRec  XsdialClassRec = {
     /* CoreClassPart */
  {
   (WidgetClass) &widgetClassRec,  /* superclass            */
   "Dial",                         /* class_name            */
   sizeof(XsDialRec),              /* widget_size           */
   ClassInitialize,                 /* class_initialize      */
   NULL,                           /* class_part_initialize */
   FALSE,                          /* class_inited          */
   Initialize,                     /* initialize            */
   NULL,                           /* initialize_hook       */
   XtInheritRealize,               /* realize               */
   actionsList,                    /* actions               */
   XtNumber(actionsList),          /* num_actions           */
   resources,                      /* resources             */
   XtNumber(resources),            /* num_resources         */
   NULLQUARK,                      /* xrm_class             */
   TRUE,                           /* compress_motion       */
   TRUE,                           /* compress_exposure     */
   TRUE,                           /* compress_enterleave   */
   TRUE,                           /* visible_interest      */
   Destroy,                        /* destroy               */
   Resize,                         /* resize                */
   Redisplay,                      /* expose                */
   SetValues,                      /* set_values            */
   NULL,                           /* set_values_hook       */
   XtInheritSetValuesAlmost,       /* set_values_almost     */
   NULL,                           /* get_values_hook       */
   NULL,                           /* accept_focus          */
   XtVersion,                      /* version               */
   NULL,                           /* callback private      */
   defaultTranslations,            /* tm_table              */
   NULL,                           /* query_geometry        */
   NULL,                           /* display_accelerator   */
   NULL,                            /* extension             */
   },
      /* Dial class fields */
  {
   0,                              /* ignore                */
   }
};

WidgetClass XsdialWidgetClass = (WidgetClass) &XsdialClassRec;

extern void CvtStringToIndicatorType();
extern void CvtStringToMarkerType();
extern void CvtStringToBorderType();

static void ClassInitialize () 
{
    XtAddConverter(XtRString, XtRIndicatorType, 
        CvtStringToIndicatorType,NULL,0);
    XtAddConverter(XtRString, XtRMarkerType, 
        CvtStringToMarkerType,NULL,0);
    XtAddConverter(XtRString, XtRBorderType, 
        CvtStringToBorderType,NULL,0);
}

static void Initialize (request, new)
    XsDialWidget request, new;
{
  XGCValues values;
  XtGCMask  valueMask;
  /*
   * Make sure the window size is not zero. The Core 
   * Initialize() method doesn't do this.
   */
  if (request->core.width == 0)
    new->core.width = 100;
  if (request->core.height == 0)
    new->core.height = 100;
  /*
   * Make sure the min and max dial settings are valid.
   */
  if (new->dial.minimum >= new->dial.maximum) {
    XtWarning ("Maximum must be greater than the Minimum");
    new->dial.minimum = 0;
    new->dial.maximum = 100;
  }
  if (new->dial.position > new->dial.maximum) {
    XtWarning ("Position exceeds the Dial Maximum");
    new->dial.position =  new->dial.maximum;
  }
  if (new->dial.position < new->dial.minimum) {
    XtWarning ("Position is less than the Minimum");
    new->dial.position =  new->dial.minimum;
  }
  /*
   * Allow only MAXSEGMENTS / 2 markers
   */
  if(new->dial.markers > MAXSEGMENTS / 2){
    XtWarning ("Too many markers");
    new->dial.markers = MAXSEGMENTS / 2;
  }

  /*
   * Create the graphics contexts used for the dial face 
   * and the indicator.
   */
  valueMask = GCForeground | GCBackground | GCLineWidth | GCFont ;
  values.foreground = new->dial.foreground;
  values.background = new->core.background_pixel;
  values.line_width = new->dial.marker_thickness ;
  values.font	    = new->dial.font->fid ;
  new->dial.dial_GC = XtGetGC ( (Widget)new, valueMask, &values);  

  valueMask = GCForeground | GCLineWidth ;
  values.foreground = new->dial.indicator_color;
  values.line_width = new->dial.indicator_thickness ;
  new->dial.indicator_GC = XtGetGC ( (Widget)new, valueMask, &values);  

  valueMask = GCForeground | GCBackground | GCLineWidth ;
  values.foreground = new->core.background_pixel;
  values.background = new->dial.indicator_color;
  values.line_width = new->dial.indicator_thickness ;
  new->dial.inverse_GC = XtGetGC ( (Widget)new, valueMask, &values);   

  Resize (new);
}

static void Destroy (w)
     XsDialWidget w;
{
  XtDestroyGC (w->dial.indicator_GC);
  XtDestroyGC (w->dial.inverse_GC);
  XtDestroyGC (w->dial.dial_GC);
  XtRemoveAllCallbacks ( (Widget)w, XtNselect );
  XtRemoveAllCallbacks ( (Widget)w, XtNrelease );
}

static void Resize (w)
     XsDialWidget w;
{
  double    angle, cosine, sine, increment;
  int       i; 
  Position  start_x, start_y;
  XPoint   *ptr;
  /*
   * Get the address of the first line segment.
   */
  ptr = w->dial.segments;
  /*
   * calculate the center of the widget
   */
  w->dial.center_x = w->core.width/2; 
  w->dial.center_y = w->core.height/2;   
  /* 
   *  Generate the segment array containing the    
   *  face of the dial.    
   */ 
  increment = RADIANS(MAX_ANGLE) / (float)(w->dial.markers -1); 
  start_x = w->dial.center_x - w->dial.marker_length; 
  start_y = w->dial.center_y - w->dial.marker_length;  
  angle = RADIANS(MIN_ANGLE);  
  for (i = 0; i < w->dial.markers;i++){   
   cosine = cos(angle);   
   sine   = sin(angle); 
   ptr->x   = w->dial.center_x + w->dial.center_x * sine; 
   ptr++->y = w->dial.center_y - w->dial.center_y * cosine; 
   ptr->x   = w->dial.center_x + start_x * sine; 
   ptr++->y = w->dial.center_y - start_y * cosine;    
   angle += increment; 
 }  
 calculate_indicator_pos(w); 
} 


static void calculate_indicator_pos(w)
     XsDialWidget w;
{
  double   normalized_pos, angle;
  Position end_x, end_y;
  /*
   * Make the indicator two pixels shorter than the  
   * inner edge of the markers.
   */
  end_x = w->dial.center_x - w->dial.marker_length - 2; 
  end_y = w->dial.center_y - w->dial.marker_length - 2; 
   /*
    * Normalize the indicator position to lie between zero
    * and 1, and then convert it to an angle.
    */
  normalized_pos = (w->dial.position - w->dial.minimum)/
                 (float)(w->dial.maximum - w->dial.minimum);
  angle = RADIANS(MIN_ANGLE + MAX_ANGLE  * normalized_pos);  
   /*
    * Find the x,y coordinates of the tip of the indicator.   
    */ 
  w->dial.indicator_x = w->dial.center_x + end_x * sin(angle); 
  w->dial.indicator_y = w->dial.center_y - end_y * cos(angle); 
} 

static void Redisplay (w, event, region)
     XsDialWidget  w;
     XEvent       *event;
     Region        region;
{
  if(w->core.visible){
    /*
     * Set the clip masks in all graphics contexts.
     */
    XSetRegion(XtDisplay(w), w->dial.dial_GC, region);
    XSetRegion(XtDisplay(w), w->dial.indicator_GC, region);

    /*
     * Draw the desired border
     */
    if (w->dial.border_type==BORDER_CIRCLE) {
	XDrawArc( XtDisplay(w), XtWindow(w),
		w->dial.dial_GC,
		0,0,
		w->core.width, w->core.height,
		0, 360*64 );
    }
    else if (w->dial.border_type==BORDER_RECTANGLE) {
	XDrawRectangle( XtDisplay(w), XtWindow(w),
                w->dial.dial_GC,
                0,0,
                w->core.width, w->core.height );
    }

    /*
     * Draw the markers used for the dial face.
     */
    if (w->dial.marker_type==MARKER_LINE)
     XDrawSegments(XtDisplay(w), XtWindow(w),
                 w->dial.dial_GC, 
                 (XSegment *) w->dial.segments,
                 w->dial.markers);
    else {
     XArc *tmp;
     int i,j;
     tmp = (XArc *) XtMalloc( w->dial.markers * sizeof(XArc) );
     for ( i=1,j=0;i<w->dial.markers*2;i=i+2,j++) {
	tmp[j].x = w->dial.segments[i].x ;
	tmp[j].y = w->dial.segments[i].y ;
	tmp[j].width = tmp[j].height = w->dial.marker_thickness ;
	tmp[j].angle1 = 0 ; tmp[j].angle2 = 360*64 ; 
     }
     XDrawArcs( XtDisplay(w), XtWindow(w),
		w->dial.dial_GC, 
		tmp,
		j );
     XtFree((char*)tmp);
    }
    /*
     * Draw title string if desired 
     */
    if ( strcmp(w->dial.title,"") ) {
	int x,y,l;
	l = XTextWidth( w->dial.font, 
		w->dial.title, strlen(w->dial.title) );
	x = w->dial.center_x - l/2 ;
	y = w->core.height - 10 ;
	XDrawString( XtDisplay(w), XtWindow(w),	w->dial.dial_GC, 
		x, y, w->dial.title, strlen(w->dial.title) );
    }

    /*
     * Draw the indicator 
     */
    draw_indicator(w);
 }
} 

static Boolean SetValues (current, request, new)
     XsDialWidget current, request, new;
{
  XGCValues  values;
  XtGCMask   valueMask;
  Boolean    redraw = FALSE;
  Boolean    redraw_indicator = FALSE;
  /*
   * Make sure the new dial values are reasonable.
   */
  if (new->dial.minimum >= new->dial.maximum) {
    XtWarning ("Minimum must be less than Maximum");
    new->dial.minimum = 0;
    new->dial.maximum = 100;
  }
  if (new->dial.position > new->dial.maximum) {
    XtWarning("Dial position is greater than the Dial Maximum");
    new->dial.position =  new->dial.maximum;
  }
  if (new->dial.position < new->dial.minimum) {
    XtWarning("Dial position is less than the Dial Minimum");
    new->dial.position =  new->dial.minimum;
  }
  /*
   * If the indicator color or background color 
   * has changed, generate the GC's.
   */
 if (new->dial.indicator_color!=current->dial.indicator_color||
  new->core.background_pixel != current->core.background_pixel){
    valueMask = GCForeground | GCBackground;
    values.foreground = new->dial.indicator_color;
    values.background = new->core.background_pixel;
    new->dial.indicator_GC = XtGetGC( (Widget)new, valueMask,&values);
    values.foreground = new->core.background_pixel;
    values.background = new->dial.indicator_color;
    new->dial.inverse_GC = XtGetGC( (Widget)new, valueMask, &values);
    redraw_indicator = TRUE;     
  }
  /*
   * If the marker color has changed, generate the GC.
   */
  if (new->dial.foreground != current->dial.foreground){
    valueMask = GCForeground | GCBackground;
    values.foreground = new->dial.foreground;
    values.background = new->core.background_pixel;
    new->dial.dial_GC = XtGetGC ( (Widget)new, valueMask, &values);   
    redraw = TRUE;     
  }
  /*
   * If the indicator position has changed, or if the min/max
   * values have changed, recompute the indicator coordinates.
   */
  if (new->dial.position != current->dial.position ||
      new->dial.minimum != current->dial.minimum ||
      new->dial.maximum != current->dial.maximum){
    calculate_indicator_pos(new);
    redraw_indicator = TRUE;
  }
  /*
   * If only the indicator needs to be redrawn and
   * the widget is realized, erase the current indicator
   * and draw the new one.
   */
  if(redraw_indicator && ! redraw &&
     XtIsRealized( (Widget)new) && new->core.visible){
    XDrawLine(XtDisplay(current), XtWindow( current),
              current->dial.inverse_GC, 
              current->dial.center_x, 
              current->dial.center_y,
              current->dial.indicator_x, 
              current->dial.indicator_y);    
    if (current->dial.show_midpoint)
	XFillArc(XtDisplay(current), XtWindow(current),
	     current->dial.inverse_GC,
	     current->dial.center_x - current->dial.midpoint_thickness/2 , 
             current->dial.center_y - current->dial.midpoint_thickness/2 ,
	     current->dial.midpoint_thickness, current->dial.midpoint_thickness,
	     0,360*64 );
   if (current->dial.show_value) {
	int x,y,l;
        char v[10];

	sprintf( v, "%d", current->dial.position );
	l = XTextWidth( current->dial.font, 
		v, strlen(v) );
	x = current->dial.center_x - l/2 ;
	y = current->core.height - 20 ;
	XDrawString( XtDisplay(current), XtWindow(current),
		current->dial.inverse_GC, 
		x, y, v, strlen(v) );
   }
    
    draw_indicator(new);
      } 
  return (redraw); 
} 

static void draw_indicator(w)
     XsDialWidget  w;
{
    /*
     * Draw the indicator at its current position.
     */
    XDrawLine(XtDisplay(w), XtWindow(w),
              w->dial.indicator_GC, 
              w->dial.center_x, 
              w->dial.center_y,   
              w->dial.indicator_x,  
              w->dial.indicator_y);   
   /*
    * Draw an middle point if desired
    */
   if (w->dial.show_midpoint)
	XFillArc(XtDisplay(w), XtWindow(w),
	     w->dial.indicator_GC,
	     w->dial.center_x - w->dial.midpoint_thickness/2 , 
             w->dial.center_y - w->dial.midpoint_thickness/2 ,
	     w->dial.midpoint_thickness, w->dial.midpoint_thickness,
	     0,360*64 );

   /*
    * Draw current value as number
    */
   if (w->dial.show_value) {
	int x,y,l;
        char v[10];

	sprintf( v, "%d", w->dial.position );
	l = XTextWidth( w->dial.font, 
		v, strlen(v) );
	x = w->dial.center_x - l/2 ;
	y = w->core.height - 20 ;
	XDrawString( XtDisplay(w), XtWindow(w),	w->dial.dial_GC, 
		x, y, v, strlen(v) );
   }
}

static void select_dial (w, event)
     XsDialWidget      w;
     XEvent           *event;
{
  Position   pos;
  double     angle;
  int help;
  
  pos = w->dial.position;
  if(event->type == ButtonPress || 
         event->type == MotionNotify){
    /* 
     * Get the angle in radians.
     */
    angle= atan2((double)(event->xbutton.y - w->dial.center_y),
                 (double)(event->xbutton.x - w->dial.center_x));
    /*
     * Convert to degrees from the MIN_ANGLE.
     */ 
    angle = DEGREES(angle) - (MIN_ANGLE - 90.0); 
    if (angle < 0)
          angle = 360.0 + angle;
    /*  
     * Convert the angle to a position. 
     */ 
    pos = w->dial.minimum + (angle /              MAX_ANGLE * 
			     (w->dial.maximum - w->dial.minimum));  
  }  
  /*
   * Use the position as the call_data to the callback list. 
   */  
  help = (int)pos;
  XtCallCallbacks ( (Widget)w, XtNselect, (caddr_t) help); 
} 

static void drag_dial (w, event)
     XsDialWidget      w;
     XEvent           *event;
{
  Position   pos;
  double     angle;
  int help;
  
  pos = w->dial.position;
    /* 
     * Get the angle in radians.
     */
    angle= atan2((double)(event->xbutton.y - w->dial.center_y),
                 (double)(event->xbutton.x - w->dial.center_x));
    /*
     * Convert to degrees from the MIN_ANGLE.
     */ 
    angle = DEGREES(angle) - (MIN_ANGLE - 90.0); 
    if (angle < 0)
          angle = 360.0 + angle;
    /*  
     * Convert the angle to a position. 
     */ 
    pos = w->dial.minimum + (angle /              MAX_ANGLE * 
			     (w->dial.maximum - w->dial.minimum));  
     
  /*
   * Use the position as the call_data to the callback list. 
   */  
  help = (int)pos;
  XtCallCallbacks ( (Widget)w, XtNselect, (caddr_t) help); 
}

Widget XtCreateDial( Widget parent, char *name, ArgList args, int argCount )
{
	return( XtCreateWidget( name, XsdialWidgetClass,
			parent, args, argCount ) );
}


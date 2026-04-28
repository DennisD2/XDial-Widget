
#include <stdio.h>
#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "DialP.h"
#include "Dial.h"

void CvtStringToIndicatorType( args, nargs, fromVal, toVal)
     XrmValuePtr args, fromVal, toVal;
     int        *nargs;
{
  static int result;

    /*
   * Make sure the number of args is correct.
   */
  if (*nargs != 0)
    XtWarning("String to IndicatorType conversion needs no arguments");
  /*
   * Convert the string in the fromVal to a int repesenting 
   */
  if (strcmp( (char*)fromVal->addr, Xt_INDICATOR_LINE ) == 0 ) 
	result= INDICATOR_LINE;
  else  if (strcmp( (char*)fromVal->addr, Xt_INDICATOR_ARROW ) == 0 )
	result= INDICATOR_ARROW;
  else
	XtStringConversionWarning((char *) fromVal->addr, "IndicatorType" );

  toVal->size = sizeof(int);
  toVal->addr = (caddr_t) &result;
} 

void CvtStringToMarkerType( args, nargs, fromVal, toVal)
     XrmValuePtr args, fromVal, toVal;
     int        *nargs;
{
  static int result;

    /*
   * Make sure the number of args is correct.
   */
  if (*nargs != 0)
    XtWarning("String to MarkerType conversion needs no arguments");
  /*
   * Convert the string in the fromVal to a int repesenting 
   */
  if (strcmp( (char*)fromVal->addr, Xt_MARKER_LINE ) == 0 ) 
	result= MARKER_LINE;
  else  if (strcmp( (char*)fromVal->addr, Xt_MARKER_POINT ) == 0 )
	result= MARKER_POINT;
  else
	XtStringConversionWarning((char *) fromVal->addr, "MarkerType" );

  toVal->size = sizeof(int);
  toVal->addr = (caddr_t) &result;
} 

void CvtStringToBorderType( args, nargs, fromVal, toVal)
     XrmValuePtr args, fromVal, toVal;
     int        *nargs;
{
  static int result;

    /*
   * Make sure the number of args is correct.
   */
  if (*nargs != 0)
    XtWarning("String to BorderType conversion needs no arguments");
  /*
   * Convert the string in the fromVal to a int repesenting 
   */
  if (strcmp( (char*)fromVal->addr, Xt_BORDER_NONE ) == 0 ) 
	result= BORDER_NONE;
  else  if (strcmp( (char*)fromVal->addr, Xt_BORDER_CIRCLE ) == 0 )
	result= BORDER_CIRCLE;
  else  if (strcmp( (char*)fromVal->addr, Xt_BORDER_RECTANGLE ) == 0 )
        result= BORDER_RECTANGLE;
  else
	XtStringConversionWarning((char *) fromVal->addr, "BorderType" );

  toVal->size = sizeof(int);
  toVal->addr = (caddr_t) &result;
} 

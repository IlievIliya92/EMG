#ifndef _CMD_INTERP_H_
#define _CMD_INTERP_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "generic_t.h"

/*********************************** DEFINES **********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
genericTask_t *getCmdInterpTask(void);

#ifdef __cplusplus
}
#endif

#endif /* _CMD_INTERP_H_ */

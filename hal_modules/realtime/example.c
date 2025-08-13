
#include "rtapi.h"		  // RTAPI realtime OS API 
#include "rtapi_app.h"	// RTAPI realtime module decls 
#include "hal.h"	    	// HAL public API decls 


/* 
   keith tries to code a linuxcnc module  
   derived from here:
   https://forum.linuxcnc.org/24-hal-components/40339-developing-hal-component-in-c
*/


MODULE_AUTHOR("keith");
MODULE_DESCRIPTION("first test");
MODULE_LICENSE("GPL");

typedef struct {
    int counter_limit_top;
    int counter_limit_bottom;
    int increment;
    hal_bit_t *up;  /* true-->up false-->down*/
    hal_bit_t *start; //true-->run false-->stop */
    hal_s32_t *counter;
} hal_example;


static hal_example *p_exmple_data;
static int comp_id;		/* component ID */
static void tomtom_realtimefunction(void *arg, long period);
static int _counter_value;

int rtapi_app_main(void)
{
  rtapi_print("--------------------------> STEP 1\n");
  comp_id = hal_init("example test");
  if (comp_id < 0) 
  {    
    rtapi_print_msg(RTAPI_MSG_ERR,
    "Tom Test: ERROR: hal_init() failed\n");
    return -7;
  }
  p_exmple_data = hal_malloc(sizeof(hal_example));
  if (p_exmple_data == 0)
  {
      rtapi_print_msg(RTAPI_MSG_ERR,
                	"Tom Test: ERROR: hal_malloc() for p_tom_dat \n");
                    hal_exit(comp_id);
	  hal_exit(comp_id);
      return -11;
  }

  // init
  p_exmple_data->increment = 1;
  p_exmple_data->counter_limit_bottom = -100;
  p_exmple_data->counter_limit_top	   =  100;
  p_exmple_data->increment 		   =    1;
  
  // export pins
  rtapi_print("--------------------------> STEP export pins\n");
  int ret_val = hal_pin_bit_new("TomTomCounter.start/stop", HAL_IN, &(p_exmple_data->start),comp_id);
  if (ret_val != 0)
  {
    return -12;
  }

  rtapi_print("--------------------------> DONE export pins\n");
  ret_val =   hal_pin_s32_new("TomTomCounter.Counter", HAL_OUT,&(p_exmple_data->counter),comp_id);
  if (ret_val != 0)
  {
    return -14;
  }

  rtapi_print("--------------------------> STEP export hal_export_funct\n");
  ret_val = hal_export_funct("Tom Tom",tomtom_realtimefunction, &p_exmple_data, 0, 0,comp_id);
  if (ret_val != 0) 
  {
    rtapi_print_msg(RTAPI_MSG_ERR,
                	"WEIGHTED_SUM: ERROR: group %d export failed\n");
                    hal_exit(comp_id);
    return -15;
  }
  rtapi_print("--------------------------> DONE export hal_export_funct\n");
  
  rtapi_print("--------------------------> CALL hal_ready\n");
  hal_ready(comp_id);
  rtapi_print("--------------------------> DONE CALL hal_ready\n");
  
  return 0;
}

void rtapi_app_exit(void)
{
    rtapi_print("--------------------------> EXIT called\n");
    hal_exit(comp_id);
}

void tomtom_realtimefunction(void *arg, long period)
{
    rtapi_print("-cyclic\n");
    return;
    p_exmple_data = (hal_example*)arg;
    if (p_exmple_data->start)
        p_exmple_data->counter++;
    rtapi_print_msg(RTAPI_MSG_ERR, "Hallo");
}
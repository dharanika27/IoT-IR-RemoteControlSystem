/*
 * ir_func.h
 *
 *  Created on: 05-Jul-2022
 *      Author: Blaze
 */

#ifndef MAIN_IR_FUNC_H_
#define MAIN_IR_FUNC_H_

/*
 * Function is called to initialize IR based GPIOS for learning and repeating
 * This function call should be made once in the program.
 * If this function is not initialize then IR_Learn and IR_Repeat function will not work.
 * @parameters 1- 0 No decoding chip
 * 				  1 with decoding chip
 * @return		- No return value
 */
void init_ir_func(char );
/*
 * Function is called whenever you want to learn a IR key.
 * After calling the function a Red LED (L1 near IR learner on
 * 				top of the board) will glows till it returns
 * 				from the function.
 * Make sure that init_ir_func is called once in the program
 * 				before calling the function
 * @parameters 1- Number of seonds that the device should be in learn mode
				- Minimum limit 5 seconds
				- Maximum limit 60 seconds
 * @return		- It returns IR learn success or fail or time out.
 * 				- 1 IR learn start success
 * 				- 2 already in learning mode or repeat mode
 *       		- 3 Learn time is less than 5 Seconds which is not allowed
 *				- 4 Learn time is more than 60 Seconds which is not allowed
 */
int IR_Learn(char Learn_time_val);
/*
 * Function is called whenever you want to repeat the IR key.
 * After calling the function a Red LED (L1 near IR learner on
 * 				top of the board) will glows till it returns
 * 				from the function.
 * Make sure that init_ir_func is called once in the program
 * 				before calling the function
 * @parameter 1 - is an with IR data with size less than 1024.
 * @parameter 2 - is used to indicate IR data type.
 * 				- if 0 ir data is from could.
 * 				- if 1 ir data is from learn data.
 * @return		- 0 WRONG_KEY
 * 				  1 KEY_FIRE_SUCCESS
 * 				  2 invalid key length
 * 				  3 invalid key
 */
int IR_Repeat(char *,unsigned char);

/*
 * This function is called after calling IR learn function.
 * 					Whenever learning is completed then the
 * 					function gives you the data of IR.
 * @parameter 1 - 1 Key learn success.
 * 				  2 timeout.
 * 				  3 data validation fail
 * 				  4 frequency not correct
 * @parameter 2 - IR data in string format. In fail case data is NULL
 */
void ir_key_learn_status(int status,char *IR_Data);
/*
 *This function is called to know the SDK version
 *@parameter 1 - will return the SDK version in string format
 */
void get_ir_idf_version(char *sdk_version);

#endif /* MAIN_IR_FUNC_H_ */

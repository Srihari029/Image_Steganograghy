#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char * argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    // Check for operation type, if operation type is encode  ---> Encode operation type
    if(check_operation_type(argv,argc) == e_encode)
    {
	    // Check the entered no.of arguments and file type are valid 
	    if(read_and_validate_encode_args(argv, argc, &encInfo) == e_success)
	    {
                  // Perform encode ---> file opening, check capacity, copy header, encode data...
		  if(do_encoding(&encInfo) == e_success)
		  {
			  printf("\n");
			  printf("Encoding Successful !!\n");
		  }
		  else
			  printf("Encoding Failed !!\n");
 	    }
	    else
	    {
		  printf("Unsupported file format\n");
		  printf("Encode Format : ./a.out  -e  <.bmp_image_file>  <Secret_message_file>  <Destination_image_file>[optional]\n");
		  printf("Decode Format : ./a.out  -d  <.bmp_image_file>  <Text_file_name>[optional]\n");  
	    }
    }

    // If operation type decode ---> Decode operation type
    else if(check_operation_type(argv,argc) == e_decode)
    {
	    // Check the entered no.of arguments and file type are valid
	    if(read_and_validate_decode_args(argv, argc, &decInfo) == e_success)
	    {
		  // Perform decode ---> file opening, decode magic string, skip header, encode data...
             	  if(do_decoding(&decInfo) == e_success)
                  {
                          printf("\n");
                          printf("Decoding Successful !!\n");
                  }
                  else
                          printf("Decoding Failed !!\n"); 
            }
            else
	    {
                  printf("Unsupported file format\n");	
                  printf("Encode Format : ./a.out  -e  <.bmp_image_file>  <Secret_message_file>  <Destination_image_file>[optional]\n");
                  printf("Decode Format : ./a.out  -d  <.bmp_image_file>  <Text_file_name>[optional]\n");
	    }
    }	

    // If operation type is neither encode nor decode ---> unsupported operation type
    else
    {
	    printf("Invalid arguments (or) Invalid number of arguments\n");
            printf("Encode Format : ./a.out  -e  <.bmp_image_file>  <Secret_message_file>  <Destination_image_file>[optional]\n");
            printf("Decode Format : ./a.out  -d  <.bmp_image_file>  <Text_file_name>[optional]\n");
    }
    return 0;
}

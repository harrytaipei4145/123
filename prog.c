

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>





int main(int argc, const char *argv[])
{
	signed char *backing;
	const char *backing_filename = argv[1];
	int backing_fd = open(backing_filename, O_RDONLY);
	backing =mmap(0, 65536, PROT_READ, MAP_PRIVATE, backing_fd, 0);
	const char *input_filename = argv[2];
	FILE *input_fp = fopen(input_filename, "r");
	FILE *output_ph = fopen("0211261_physical.txt", "w");
	FILE *output_va = fopen("0211261_value.txt", "w");
	int tlb_tag[16], tlb_page_number[16];
	int page_table[256];
	int virtual_address,i=0;
	int physical[500];
	signed char value[500];
    int page_number[500],offset[500];
    int file_input=0;
    int tlb_max=-1,page_max=-1,tlb_count=-1,page_count=0;
    int tlb_hit=0,fault=0;
    int execute_number=0;
    int tlb_check=0,page_check=0;

   	
   	
    while (!feof(input_fp)){
     fscanf(input_fp,"%d",&virtual_address);
     page_number[file_input]=virtual_address/256;
     offset[file_input]=virtual_address-page_number[file_input]*256;     
     file_input++;
    }

    while(execute_number<=499)
    {
           tlb_check=0;
           page_check=0;
           if(tlb_max>=0)
           {
                         for(i=0;i<=tlb_max;i++)
              {
                                 if(page_number[execute_number]==tlb_tag[i])
                                 {
                                                                            tlb_hit++;
                                                                            tlb_check=1;
                                                                            physical[execute_number]=tlb_page_number[i]*256+offset[execute_number];
                                                                            value[execute_number]=backing[page_number[execute_number]*256+offset[execute_number]];
                                                                            
                                 }
               }
           }
           if(tlb_check==0)
           {
                       for(i=0;i<=page_max;i++)
                      {
                                 if(page_number[execute_number]==page_table[i])
                                 {
                                                                            page_check=1;
                                                                            physical[execute_number]=i*256+offset[execute_number];
                                                                            value[execute_number]=backing[page_number[execute_number]*256+offset[execute_number]];
                                                                            if(tlb_max<15)
                                                                            {
                                                                                          tlb_max++;
                                                                                          tlb_count++;
                                                                                          tlb_tag[tlb_count]=page_number[execute_number];
                                                                                          tlb_page_number[tlb_count]=i;
                                                                                                  
                                                                            }
                                                                            else if(tlb_max==15)
                                                                            {
                                                                                 tlb_count++;
                                                                                 if(tlb_count==16)
                                                                                 tlb_count=0;
                                                                                 tlb_tag[tlb_count]=page_number[execute_number];
                                                                                 tlb_page_number[tlb_count]=i;
                                                                                 
                                                                            }
                                 }
                      }
                      if(page_check==0)
                      {
                                       fault++;
                                       page_max++;
                                       page_table[page_max]=page_number[execute_number];
                                       physical[execute_number]=page_max*256+offset[execute_number];
                                       value[execute_number]=backing[page_number[execute_number]*256+offset[execute_number]];
                                       if(tlb_max<15)
                                                                            {
                                                                                          tlb_max++;
                                                                                          tlb_count++;
                                                                                          tlb_tag[tlb_count]=page_number[execute_number];
                                                                                          tlb_page_number[tlb_count]=page_max;
                                                                                                  
                                                                            }
                                                                            else if(tlb_max==15)
                                                                            {
                                                                                 tlb_count++;
                                                                                 if(tlb_count==16)
                                                                                 tlb_count=0;
                                                                                 tlb_tag[tlb_count]=page_number[execute_number];
                                                                                 tlb_page_number[tlb_count]=page_max;
                                                                                 
                                                                            }
                                       
                                       
                                       
                      }
                       
           }
           fprintf(output_ph, "Physical address: %d\n",physical[execute_number] );
           fprintf(output_va, "Value: %d\n",value[execute_number]);
           printf("Virtual address: %d Physical address: %d Value: %d\n",page_number[execute_number]*256+offset[execute_number],physical[execute_number],value[execute_number]);
           execute_number++;
           
    }
   
                         
                       
    
 
    printf("Number of Translated Addresses = 500\n");
    printf("Page Faults = %d\n", fault);
    printf("Page Fault Rate = %.3f\n", fault / 500.000);
    printf("TLB Hits = %d\n", tlb_hit);
    printf("TLB Hit Rate = %.3f\n", tlb_hit /500.000);



}

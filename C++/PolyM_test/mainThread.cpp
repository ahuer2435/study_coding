#include "CommonParam.h"
#include <vector>
#include <string>
#include <fstream>
#include "adas_perception.h"
#include "hmi_interface.h"

pthread_t ADAS_PERCEPTION_ID;
int main(void)
{
  int Ret = pthread_create(&ADAS_PERCEPTION_ID,NULL,bstAdas_perception_Handler,NULL);
  if(Ret)
  {
    std::cout << "Failed to create adas perception thread!" << std::endl;
    return 1;
  }
  std::cout <<"adas perception Thread created!" << std::endl;
  HMI_dispaly();
  pthread_join(ADAS_PERCEPTION_ID,NULL);
  return 0;
}
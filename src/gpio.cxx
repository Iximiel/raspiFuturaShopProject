#include <iostream>
#include <string>
#include <gpiod.hpp>
#include <chrono>
#include <thread>

int main (int, char**argv) {
  int retval =0;
  try{
    gpiod::chip GPIO("pinctrl-bcm2835");
    /*for( int i =0; i<GPIO.num_lines();++i) {
      std::cout << i
		<< '\t' << GPIO.get_line(i).name()
		<< '\t' << GPIO.get_line(i).direction()
		<< '\t' << GPIO.get_line(i).is_used()
		<< std::endl;
    }*/
    /*
      GPIO.get_line(24).set_value(1);
      std::this_thread::sleep_for (std::chrono::seconds(1));
      GPIO.get_line(24).set_value(0);
    */
    auto line = GPIO.get_line(24);
    if (!line.is_requested()){      
      line.request({
		    argv[0],
		    ::gpiod::line_request::DIRECTION_OUTPUT,
		    0
	}, 0);
      //line.set_value(0);
      std::this_thread::sleep_for (std::chrono::seconds(1));
            line.set_value(1);
	    /*
      line.request({
		    argv[0],
		    ::gpiod::line_request::DIRECTION_OUTPUT,
		    0
		    }, 0);*/
    }
  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  }
  return retval;
}

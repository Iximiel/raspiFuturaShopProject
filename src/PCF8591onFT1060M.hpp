#ifndef MYPCF8591_HPP
#define MYPCF8591_HPP
namespace FT1060M {
  class PCF8591 {
  public:
    static PCF8591& getPCF8591();
    PCF8591(const PCF8591& )=delete;
    PCF8591(const PCF8591&& )=delete;
    PCF8591& operator=(const PCF8591& )=delete;
    PCF8591& operator=(const PCF8591&& )=delete;
    int readFromAnalogChannel0(const int& times=3);
    int readFromAnalogChannel1(const int& times=3);
    int readFromAnalogChannel2(const int& times=3);
    int readFromAnalogChannel3(const int& times=3);
    int writeToAnalogOut(const int& value);
    void setAnalogOutputEnabled(const bool&);
    bool getAnalogOutputEnabled() const;
    static constexpr int adapterNumber = 1;
    static constexpr int i2CAddress = 0x48;
  private:
    PCF8591();
    int readFromAnalogChannel(const int& channel,const int& times);
    enum masks {
	  AnalogOutputEnableMask = 1 << 6,
	  AnalogInputModeMask = 1<<5|1<<4,
	  AutoIncrementMask = 1<<2,
	  ADchannelMask = 1<<1 | 1
    };
    //state
    int deviceFile_;
    bool AnalogOutputEnabled_{false};  
  };
}
#endif // MYPCF8591_HPP

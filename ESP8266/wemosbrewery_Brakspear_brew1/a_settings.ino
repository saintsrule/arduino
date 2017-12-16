//Time basic settings

int startingHour = 0; // set your starting hour here, not at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 0;
int days = 0;

//Time ccuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketc

//Blynk and wifi

//char auth[] = "ec8022b955cb4f4b8ee688209ada29d3";

//How many KiloWatt are your heating elements combined?

int KiloWatt = 3; 

//Temperature and times in recipe:


int mashTemp = 65; // Temp in celcius  // Was 65
int boilTemp = 97; // Temp in celcius, set to max 97  // Miles reduced to 97 because boiling occurs at 98max
// int boilTemp = 99; // Temp in celcius, set to max 99
int mashTime = 30; // number of minutes you are going to mash


int boilTime = 90; // number of minutes you are going to boil
int fermentTimeprimary = 14; // Number of days you are going to ferment
int fermentTimeSecondary = 14; // Number of days you are going to ferment in secondary, set to zero if you do not plan on using secondary fermenting, this is often not required. 


int yeastPitchTemperature = 24; // The ideal pitch temperature for your yeast, in celcius.

int fermentationTemperatureMin = 20; // Minimum Temp that the yeast can thrive in, in celcius.  
int fermentationTemperatureMax = 24; // Maximum Temp that the yeast can thrive in, in celcius.  

int HopTime1 = 80;; // after how many minutes of boiling the wort should this hop be added? 
int HopTime2 = 90;;
int HopTime3 = 90;
int HopTime4 = 90;





//Liters you will make

int liters = 50; //The number of liters of boiled wort you are planning on having AFTER the boil.

// Brew stages, Adjust this if you faced a power down, or reset at some point during the brew, otherwise leave at zero. 

int brewStage = 6;// 0 is standby, 1 is heating strike water, 2 is waiting for mash, 3 is mashing, 4 is extracting mash and waiting for boil, 5 is heating towards boil, 6 is boil, 7 is Ice cooling, 8 is adding yeast, 9 is fermenting primary, 10 is fermenting secondary change this number here to skip one or several of the stages
                    



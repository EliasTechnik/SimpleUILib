#include <Arduino.h>
#include <U8g2lib.h>

#include "simpleUI.h" //include the ui library

//include files for images and icons
//To save on memory the bitmaps must be manualy included.
#include "icons/8x8/flash.xbm"
#include "icons/8x8/network.xbm"
#include "icons/128x64/logo.xbm"
#include "icons/10x10/leftArrow.xbm"

//some pin definitions
#define BACK_BUTTON 32 
#define LEFT_BUTTON 33
#define RIGHT_BUTTON 25
#define ENTER_BUTTON 26

using namespace std;

//initialize the display. See the U8g2 documentation for more information.
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);//OLED UI

//Define the UI Globals
uiRoot* UI; //the root of the UI (tree)
uiHardwareInputDriver* HID; //the hardware input driver
uiHardwareButton* BackButton; //the hardware button for the back button
uiHardwareButton* LeftButton; //the hardware button for the left button
uiHardwareButton* RightButton; //the hardware button for the right button
uiHardwareButton* EnterButton; //the hardware button for the enter button

//currently the libary does define own ISRs. It has to be done manually.
//the ESP32 needs IRAM_ATTR for ISRs so we define them here
void IRAM_ATTR b_back_ISR(){
  BackButton->trigger();
}
void IRAM_ATTR b_left_ISR(){
  LeftButton->trigger();
}
void IRAM_ATTR b_right_ISR(){
  RightButton->trigger();
}
void IRAM_ATTR b_enter_ISR(){
  EnterButton->trigger();
}


//for ui buttons we need some callback functions
void cbButton1(void * sender, UIEventType event_type){
  //the sender is the uiElement that triggered the event. The event_type is the type of event that was triggered.
  //for debugging we can use the following functions to print the event type. 
  Slog(UIEventTypeToString(event_type));
  Slog(UIEventTypeToStringExplenation(event_type));
  Slog("CB_Button 1!")
}
void cbButton2(void * sender, UIEventType event_type){
  Slog("CB_Button 2!")
}
void cbButton3(void * sender, UIEventType event_type){
  Slog("CB_Button 3!")
}
void cbIconButton1(void * sender, UIEventType event_type){
  Slog("CB_IconButton 1!")
  //the HardwareInputDriver is explained later. 
  //All you need to knwo for now is that it handles user Inputs to the UI. By injecting an input we can simulate a button press.
  HID->injectInput(UserAction::backButton);
}

void cbCheckBox1(void * sender, UIEventType event_type){
  Slog("CB_Checkbox 1!")
  Slog(UIEventTypeToString(event_type));
  Slog(UIEventTypeToStringExplenation(event_type));
}

void cbCheckBox2(void * sender, UIEventType event_type){
  Slog("CB_Checkbox 2!")
  Slog(UIEventTypeToString(event_type));
  Slog(UIEventTypeToStringExplenation(event_type));
}

void cbValueChange(void * sender){
  Slog("the value changed to:")
}

//This is a callback function for the radio group. It is called when the checked radio changes.
//The sender is the radio group and the event_type is the event that triggered the callback.
void cbRadioChange(void * sender, UIEventType event_type){
  Slog("CB_RadioGroup!")
  Slog(UIEventTypeToString(event_type));
  Slog(UIEventTypeToStringExplenation(event_type));
  uiRadioGroup * rg = (uiRadioGroup*)sender;
  Serial.print("Checked Radio: ");
  Serial.println(rg->getCheckedRadioIndex());
}

//setting up the UI can lead to quite some code. This is why we put it in a separate functions.
//customized ui components can be created by combining existing components and using a function to set them up.
uiSelectGroup* setupEditTest(){
  uiSelectGroup* editTest = new uiSelectGroup();

  //add editable Text
  editTest->addChild(
    new uiIntValueInput(
      Position(10,10),0,0,255,&cbValueChange
    )
  );
  editTest->addChild(
    new uiIntValueInput(
      Position(30,10),0,0,255,&cbValueChange
    )
  );
  editTest->addChild(
    new uiIntValueInput(
      Position(50,10),0,0,255,&cbValueChange
    )
  );
  editTest->addChild(
    new uiIntValueInput(
      Position(70,10),0,0,255,&cbValueChange
    )
  );

  //some icons to show off
  editTest->addChild(
    new uiImageContainer(
      10,20,8,8,network_xbm
    )
  );

  editTest->addChild(
    new uiImageContainer(
      10,30,8,8,flash_xbm
    )
  );

  return editTest;
}

//setting up the UI is best done in a separate function
void setupUI(){

  //create a new config. SimpleUI is designed to be flexible and to support multiple display types. 
  //To do this you have to create a DisplayConfig object, fill it with information and pass it to the UI root object.
  DisplayConfig config;

  //set the output device
  config.display = &u8g2;
  //change the viewport offset (this is used to adjust the display to the physical screen)
  config.viewportOffset = DEFAULT_OFFSET;

  //create a new ui root object. This is the start of the UI Tree
  UI = new uiRoot(config);
  UI->showStartupScreen(); //This shows a simple startup screen. It is optional but can be used to show the user that the device is booting. TODO: make it customizable
  //Note: The Startupscreen is expected to be shown before the first call of UI->display(). During this time the display is not updated and therefore no burn in protection is active. 
  //DO NOT USE THIS FUNCTION IF YOUR SETUP TAKES LONGER THAN 20 SECONDS TO RUN!

  //create some groups to structure the UI. uiSelectGroups are used to group elements that can be selected.
  uiSelectGroup* mainGroup = new uiSelectGroup();
  mainGroup->setID("mainGroup"); //uiElements can be identified by their ID. This is usefull for debugging and for callbacks but not neccessary.
  uiSelectGroup* secGroup = new uiSelectGroup();
  secGroup->setID("secGroup");
  uiRadioGroup* radioGroup = new uiRadioGroup(); //when usinmg radio buttons you should use a uiRadioGroup instead of a uiSelectGroup because it has additional functionality for managing the radio logic.
  secGroup->setID("thirdGroup");

  //add some ui elements to the Groups
  //uiPassiveLabels are used to display text. They are passive and can not receive focus.
  uiPassiveLabel * label = new uiPassiveLabel("Input Mode",Position(25,0)); //Position is used to set the position of the element in X and Y direction.
  label->setID("label");
  mainGroup->addChild(
    label
  );

  //uiBasicButtons are used to create buttons. They can be selected and can receive focus. The later means they can be pressed by the user. Buttons require a callback function.
  uiBasicButton * bt1 = new uiBasicButton(Position(10,10),"High",&cbButton1);
  bt1->setID("opt1");
  mainGroup->addChild(
    bt1,true
  );
  uiBasicButton * bt2 = new uiBasicButton(Position(10,25),"Medium",&cbButton2);
  bt2->setID("opt2");
  mainGroup->addChild(
    bt2,true
  );
  
  uiBasicButton * bt3 = new uiBasicButton(Position(10,40),"Low",&cbButton3);
  bt3->setID("opt3");
  mainGroup->addChild(
    bt3,true
  );

  uiIconButton * bt4 = new uiIconButton(Position(110,50),new uiImage(10,10, leftArrow_xbm),&cbIconButton1);
  bt4->setID("IconButton1");
  mainGroup->addChild(
    bt4,true
  );

  //if you dont need to access the element later you can add it directly to the group
  secGroup->addChild(
    new uiPassiveLabel("Test",Position(32,0))
  );

  //checkboxes with labels are also supported
  secGroup->addChild(
    new uiCheckbox(Position(10,10),"Checkbox 1", false, &cbCheckBox1)
  );

  secGroup->addChild(
    new uiCheckbox(Position(10,25),"Checkbox 2", true, &cbCheckBox2) //the third parameter is the initial state of the checkbox
  );



  //radioButtons are also supported. They are used to create a group of selectable elements where only one can be selected at a time.
  //You could use them as standalone elements but they are more powerfull when used in a uiRadioGroup.
  radioGroup->addRadio(
    new uiRadio(Position(10,10),"Radio 1", true) //the third parameter is the initial state of the checkbox
  );
  radioGroup->addRadio(
    new uiRadio(Position(10,25),"Radio 2", false) //uiRadio doesnt require a callback because they can be managed by its Parent Group (uiRadioGroup)
  );
  radioGroup->addRadio(
    new uiRadio(Position(10,40),"Radio 3", false)
  );

  radioGroup->setOnChange(&cbRadioChange); //the callback is triggered when the checked radio changes.


  //SimpleUI comes with some basic geometric shapes which can be used to create custom graphics and/or components.
  //In order to use them without defining a new component you can add them to a uiGraphicCanvas.
  uiGraphicCanvas* canvas = new uiGraphicCanvas();

  //uiGraphicCanvas needs the graphic shapes to be wrapped in uiGraphicElement. 
  //This is neccessary because the shape Object does not store dimensional information.
  //draw a circle
  canvas->addGraphic(new uiGraphicElement(
    new uiCircle(3),
    Dimension(32,32,10,10)
    )
  );

  //draw a filled circle
  canvas->addGraphic(new uiGraphicElement(
    new uiFilledCircle(),
    Dimension(16,16,10,10)
    )
  );
  
  //draw a horizontal line
  canvas->addGraphic(new uiGraphicElement(
    new uiLine(1),
    Dimension(16,48,100,48,true)
    )
  );

  //draw a angled line (currently linewidths >1 on angled lines are not supported)
  canvas->addGraphic(new uiGraphicElement(
    new uiLine(3),
    Dimension(4,4,124,4,true)
    )
  );

  //draw another angled line
  canvas->addGraphic(new uiGraphicElement(
    new uiLine(1),
    Dimension(16,50,100,64)
    )
  );

  //draw a thick line
  canvas->addGraphic(new uiGraphicElement(
    new uiLine(4),
    Dimension(120,4,120,60)
    )
  );


  //full size images a also supported. They can be used to display bitmaps and xbm images.
  //to show this of we crate a page with a full size image.
  uiPage* splashScreen = new uiPage(new uiImageContainer(0,0,128,64,logo_XBM));

  //uiPages are the elements behind uiRoot. They can be used to create a pagination. Currently the are not stackable. 
  //This means Pagination is only available on Root level.
  //on each page we add a priveous created group or uiElement.
  uiPage* page1 = new uiPage(mainGroup);
  page1->setID("page1");
  uiPage* page2 = new uiPage(secGroup);
  page2->setID("page2");
  uiPage* page3 = new uiPage(canvas);
  page3->setID("page3");
  uiPage* page4 = new uiPage(radioGroup);

  //now wee add the pages to the UI root
  //the page first added is the first page shown.
  UI->addPage(splashScreen);
  UI->addPage(new uiPage(setupEditTest())); //as mentioned before we can create custom components by combining existing ones and returning a uiElement in a function. 
  UI->addPage(page1);
  UI->addPage(page2);
  UI->addPage(page3);
  UI->addPage(page4);

  //some debug output of some pages and elements
  Slog(page1->getConfig());
  Slog(page2->getConfig());
  Slog(mainGroup->getConfig());
  Slog(secGroup->getConfig());
}


//now its time to setup the hardware and the UI
//this is the arduion setup function
void setup() {
  //set up some pins and attach the ISRs
  pinMode(BACK_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(ENTER_BUTTON, INPUT_PULLUP);
  attachInterrupt(BACK_BUTTON, b_back_ISR, CHANGE);
  attachInterrupt(LEFT_BUTTON, b_left_ISR, CHANGE);
  attachInterrupt(RIGHT_BUTTON, b_right_ISR, CHANGE);
  attachInterrupt(ENTER_BUTTON, b_enter_ISR, CHANGE);

  //start the serial connection for debugging
  Serial.begin(115200);

  //start the display
  u8g2.begin();

  //setup the UI (this is the function we defined above)
  setupUI();

  //create the hardware input driver and connect it to the UI
  HID = new uiHardwareInputDriver(UI);

  //create the hardware buttons and connect them to the HID
  BackButton = new uiHardwareButton(BACK_BUTTON,UserAction::backButton);
  LeftButton = new uiHardwareButton(LEFT_BUTTON,UserAction::leftButton);
  RightButton = new uiHardwareButton(RIGHT_BUTTON,UserAction::rightButton);
  EnterButton = new uiHardwareButton(ENTER_BUTTON,UserAction::enterButton);
  EnterButton->configureLongpress(UserAction::backButton); //this is an example of how to configure a longpress. The first parameter is the action that is triggered on longpress.
  RightButton->configureAutopress(5); //this is an example of how to configure an autopress. The first parameter is the speed of the autopress. (ms between a press trigger)
  EnterButton->switchLongpress(true); //Lonpresses are considered secondary inputs and can be switched on or off. Default is off so we enable it here.
  RightButton->switchAutopress(true); //Autopresses are considered secondary inputs and can be switched on or off. Default is off so we enable it here. 
  /*
  Note that autopresses and longpresses cant be enabled at the same time. Swiching ether of them on will disable the other.
  */


  HID->addInput(BackButton);
  HID->addInput(LeftButton);
  HID->addInput(RightButton);
  HID->addInput(EnterButton);
  Slog("Setup done");
  //sleep(10);
}

//this is the arduino loop function. In here we have to do two things:
//1. process the hardware inputs
//2. display the UI
//This has to be done frequently to ensure a smooth user experience. The HID should run as often as possible to prevent missed inputs.
//The UI draws in fixed intervals based on the provided display configuration. Calling UI->display() to frequently cant harm but calling it to seldom can lead to a laggy experience and low FPS.
void loop() {
  HID->run(); //this processes user inputs
  UI->display(); //this draws the UI and also manages the screen (prevents burn in)
}
/******************************************************************
 * ArduinoANN - An artificial neural network for the Arduino
 * All basic settings can be controlled via the Network Configuration
 * section.
 * See robotics.hobbizine.com/arduinoann.html for details.
 ******************************************************************/

#include <math.h>
#include <stdio.h>

/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/

const int PatternCount = 1;
const int HiddenNodes = 10;
const int InputNodes = 8;
const int OutputNodes = 1;

//DATA, correspondiente a 10 datos del Google Colab
const float x[8][8] = {
  //    HOUR   TEMPERATURE  HUMIDITY  WIND SPEED VISIBI   DEW P. TEM  SOLAR RAD  SEASONS
  { -0.304348,  0.573427,  0.408163, -0.513514, 0.998986,  0.785467, -0.602273, 0.333333},  // 0
  {  0.043478,  0.762238,  0.040816, -0.432432,-0.216422,  0.788927,  0.823864, 0.333333},  // 1
  {  0.217391,  0.811189,  0.020408, -0.675676, 0.740497,  0.823529, -0.045455, 0.333333},  // 2
  {  0.565217,  0.213287, -0.040816, -0.621622, 0.632032,  0.249135, -0.937500, 1.000000},  // 3
  { -0.391304, -0.153846,  0.040816, -0.729730, 0.389762, -0.044983, -0.892045,-0.333333},  // 4
  { -0.913043,  0.601399,  0.448980, -0.837838, 0.796249,  0.826990, -1.000000, 0.333333},  // 5
  {  0.739130,  0.716783,  0.326531, -0.459459, 0.580335,  0.885813, -0.988636, 0.333333},  // 6
  {  0.913043, -0.304196, -0.163265, -0.729730, 0.857070, -0.280277, -1.000000, 1.000000},  // 7
};

const float b[8][1] = {
  //    salida Colab
  { -0.5382426 },  // 0
  { -0.54876304 },  // 1
  { -0.51805633 },  // 2
  { -0.1997118 },  // 3
  { -0.779216 },  // 4
  { -0.6631822 },  // 5
  {  0.07259303 },  // 6
  { -0.49105334 }   // 7
};

// Pesos capa oculta
const float HiddenWeights[HiddenNodes][InputNodes+1]= {
    { 0.15369269, 0.3312562,  1.4267931,  0.48202458,-0.04291498,-0.40615246, 0.14495648, 0.15664306,-0.35153377},        
    { 1.2025405, -0.225507,   0.04902516, 0.03255267,-0.07565843, 0.10837542,-0.47980785, 0.0558089,  0.34195536},         
    {-1.5895532,  0.13333608, 0.25250763,-0.16203707,-0.02767957, 0.5014229,  0.11747295,-0.07578623,-0.14157137},           
    {-0.9379399,  0.03146402,-0.1942246, -0.15585914,-0.17415945, 0.48989156, 0.17208055, 0.2120251,  0.3492393},     
    { 0.01284913,-0.43376505, 0.46243975,-0.5209553, -0.00680515,-0.47540686,-0.34646627,-0.49524125,-0.02995161},
    { 1.1240828, -0.6761673,  0.32052124, 0.02109032,-0.13230047,-0.42832443, 0.26043203, 0.11331557, 0.17965677},      
    {-0.08674003,-0.61752754,-0.27852643, 0.31043944,-0.03518743, 0.17185695,-0.02614462,-0.04802864, 0.36990458},      
    { 0.4220387, -0.7406621,  0.44058576,-0.05053395,-0.02474852,-0.7445681,  0.10145678, 0.25794557,-0.1291832},     
    {-0.74370533, 0.67488146, 0.3067462,  0.0655808 , 0.0827681,  0.43165722,-0.22072506,-0.29961932,-0.13713752},         
    { 0.19479065,-0.0470287,  0.47550586,-0.25878924, 0.09739551,-0.49912205,-0.7704878,  0.62123394,-0.450278}

};

// Pesos capa de salida
const float OutputWeights[OutputNodes][HiddenNodes+1]  = {
{-0.50396854,   0.39482683,   0.8108938,   -0.57370925,  -0.42702177,    -0.78488326,   -0.7218168,   0.863512,    -0.6874971,   -0.41498458, -0.06467228}
};  


/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r, k,o;
float Accum;
float Hidden[HiddenNodes];
float Output[OutputNodes];
float Input[InputNodes][PatternCount];
float Colab;



void setup(){
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop(){
  //Recorre las diferentes columnas para rellenar el vector Input con cada uno de los atributos
  for( o = 0 ; o < 8 ; o++ ) {
    Input[o][0] = x[k][o];
    }

   Colab = b[k][0];
/******************************************************************
* Compute hidden layer activations
******************************************************************/

    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[i][InputNodes] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += HiddenWeights[i][j]*Input[j][0];
      }
      if (Accum < 0) {
        Hidden[i] = 0; 
      }
      else {
        Hidden[i] = Accum;
      }
    }
    
/******************************************************************
* Compute output layer activations
******************************************************************/
 
    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[i][HiddenNodes] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum +=  OutputWeights[i][j]*Hidden[j];
      }
      Output[i] = Accum; 
    }

    //Contador para acceder a cada una de las filas en cada ciclo
    k++;
    if (k == 8) {
      k=0;
    }

    //Enseña en el serial los valores de las predicciones
    Serial.print("OutputTest_Arduino:");
    Serial.print(Output[0]);
    Serial.print(",");
    Serial.print("OutputTest_Colab:");
    Serial.print(Colab);
    Serial.print("\n");
    
    delay(1000);
}

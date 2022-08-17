#define in9 9
#define in10 10
#define in11 11
#define in12 12
struct motor{
  int a;
  int b;
};
motor DCmotor ;
float front_distance;
float left_distance;
float right_distance;
char state;
int safe_distance = 20;
int sensor1;
int sensor2;
int sensor3;

void setup() {
  // put your setup code here, to run once:
  pinMode(in9, OUTPUT);
  pinMode(in10, OUTPUT);
  pinMode(in11, OUTPUT);
  pinMode(in12, OUTPUT);
  Serial.begin(9600);
}
float getDistance(int trig,int echo){         // Hàm lấy khoảng cách từ cảm biến khoảng cách
  float dem=0;
  pinMode(trig,OUTPUT);
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  pinMode(echo, INPUT);
  dem = pulseIn(echo,HIGH,30000)/58.0;
  if(dem==0) dem=30;
  return dem;
}
void dieukhien (int x, int y){
    switch (x){
      case 1:
      DCmotor.a = in9;
      DCmotor.b = in10;
      break;
      case 2:
      DCmotor.a = in11;
      DCmotor.b = in12;
      break;
    }
    switch (y){
      case 1: // tien
     digitalWrite (DCmotor.a,HIGH);
     digitalWrite (DCmotor.b,LOW);
      break;
       case 2: // lui
     digitalWrite (DCmotor.a,LOW);
     digitalWrite (DCmotor.b,HIGH);
      break;
       case 3:// dung im
      digitalWrite (DCmotor.a,LOW);
      digitalWrite (DCmotor.b,LOW);
       break;
    }   
}
void dichuyen(int a, int b){
  dieukhien(1,a);
  dieukhien(2,b);
}
void go_aheard(){
  dichuyen(1, 1);
}
void go_back(){
  dichuyen(2, 2);
}
void turn_right(){
  dichuyen(3, 1);
}
void turn_left(){
  dichuyen(1, 3);
}
void stop_moving(){
  dichuyen(3, 3);
}
void auto_control()
{
  front_distance = 0;
  front_distance = getDistance(2,3);
  if (front_distance > safe_distance || front_distance == 0)
  {
      go_aheard();
  }
  else
  {
    stop_moving();
    delay(300);
    left_distance = getDistance(4,5);
    right_distance = getDistance(6,7);
    if (right_distance < 15 && left_distance < 15) 
    {
      go_back();
      delay(300);
      stop_moving();
      delay(300); 
    }
    else
    {
      if (right_distance >= left_distance)
      {go_back();
      delay(300);
      stop_moving();
      delay(300);      
      turn_right();
      delay(400);
      stop_moving();
      delay(300);
     }
    if (right_distance < left_distance)
     { 
      go_back();
     delay(300);
     stop_moving();
     delay(300);
     turn_left();
     delay(400);
     stop_moving();
     delay(300);
      }
    }
  }
}
void loop() {
 if(Serial.available() > 0){
   // Đọc giá trị nhận được từ bluetooth
    state = Serial.read();
  }
    else  state = 0;
  switch (state) {
  case '1':
    go_aheard();
    break;
  case '2':
    go_back();
    break;
  case '3':
    turn_left();
    break;
  case '4':
    turn_right();
    break;
  case '5':
    stop_moving();
    break;
   
   } 
   if ( state == '6'){
    while(1){
      auto_control();
      if(Serial.available() > 0)  state = Serial.read();   
      if(state == '7') break;
      }
    }
}

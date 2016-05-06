#include <Smartcar.h>
#include <SoftwareSerial.h>

Odometer encoderLeft(190), encoderRight(190); //Βάλτε τους δικούς σας παλμούς ανά μέτρο
Gyroscope gyro(13); //Βάλτε την κατάλληλη τιμή σύμφωνα με το γυροσκόπιό σας
Car folkracer;
SR04 sonarLeft, sonarRight, sonarFront; //dilwse tis metavlites sonarLeft, sonarRight kai sonarFront pou antiproswpevoun tous iperixous
SoftwareSerial bluetooth(6,7); //συνδέστε το bluetooth ως εξής: Το RX του Bluetooth στο pin 6 και το ΤΧ του bluetooth στο pin 7 (VCC -> 5V, GND -> GND)

/* Χρησιμοποιήστε τα σωστά pins!!!! */
const int SONAR_LEFT_TRIGGER = 4; //trigger του αριστερού υπέρηχου
const int SONAR_LEFT_ECHO = 5; //echo του αριστερού υπέρηχου
const int SONAR_RIGHT_TRIGGER = A0; //trigger του δεξιού υπέρηχου
const int SONAR_RIGHT_ECHO = A1; //echo του δεξιού υπέρηχου
const int SONAR_FRONT_TRIGGER = A2; //trigger του μπροστινού υπέρηχου
const int SONAR_FRONT_ECHO = A3; //echo του μπροστινού υπέρηχου

void setup() {
  bluetooth.begin(9600); //ξεκινήστε τη σειριακή σύνδεση
  sonarLeft.attach(SONAR_LEFT_TRIGGER, SONAR_LEFT_ECHO); // αρχικοποίησε τον υπέρηχο στα κατάλληλα pins
  sonarRight.attach(SONAR_RIGHT_TRIGGER, SONAR_RIGHT_ECHO);
  sonarFront.attach(SONAR_FRONT_TRIGGER, SONAR_FRONT_ECHO);
  gyro.attach(); //αρχικοποιεί το γυροσκόπειο
  encoderLeft.attach(3); //αρχικοποιεί το encoder
  encoderRight.attach(2);
  encoderLeft.begin();//ξεκινάει τις μετρήσεις στον encoder
  encoderRight.begin();
  folkracer.begin(encoderLeft, encoderRight, gyro); //ξεκινάει το αυτοκίνητο χρησιμοποιώντας τα encoders και το γυροσκόπειο
  folkracer.enableCruiseControl(); //ξεκινάει τον έλεγχο της ταχύτητας του αυτοκινήτου
  gyro.begin(); //ξεκινάει τις μετρήσεις στο γυροσκόπειο
  /* Εάν θέλετε να διαβάσετε απλά τις αποστάσεις, δίχως να κινείται το αυτοκινητάκι, βάλτε ταχύτητα 0 στην παρακάτω γραμμή */
  folkracer.setSpeed(0); //θέτει την ταχύτητα στο αυτοκινητάκι στα 0.2 μέτρα ανά δευτερόλεπτο (εάν είναι πολύ αργό, αυξήστε λίγο την ταχύτητα)
  folkracer.setAngle(0); //Το αυτοκινητάκι πηγαίνει ευθεία
}

void loop() {
  folkracer.updateMotors(); //διατήρησε την ταχύτητα του αυτοκινήτου σταθερή
  int frontDistance = sonarFront.getMedianDistance(3); //μέτρησε την απόσταση (φιλτράρισε 3 μετρήσεις) και αποθήκευσέ την στη μεταβλητή frontDistance
  int leftDistance = sonarLeft.getMedianDistance(3);
  int rightDistance = sonarRight.getMedianDistance(3);
  bluetooth.print(frontDistance); //εκτύπωσε τις αποστάσεις με την εξής μορφή: μπροστινήΑπόσταση,αριστεράΑπόσταση,δεξιάΑπόσταση
  bluetooth.print(",");
  bluetooth.print(leftDistance);
  bluetooth.print(",");
  bluetooth.println(rightDistance);
  //TO-DO για το σχολείο: Κινήστε το αυτοκινητάκι στην πίστα και ΑΣΥΡΜΑΤΑ δείτε τις αποστάσεις στον υπολογιστή, μέσω bluetooth (δεν χρειάζεται επιπλέον κώδικας)
  //TO-DO για το σπίτι: Κάντε το αυτοκινητάκι να προσπαθεί να μείνει στη μέση των δύο τοίχων. Το πόσο θα στρίψετε αριστερά, με το folkracer.setAngle θα το βρείτε πειραματικά. Αρχίστε τις δοκιμές με το setAngle(50).
   
  if (rightDistance == 0 && leftDistance ==0) {
    folkracer.setSpeed(0.5);
  }
  if (rightDistance ==0 && 0<leftDistance<30) {
    folkracer.setAngle(30);
  }
  if (rightDistance == 0 && leftDistance>30) {
    folkracer.setSpeed(0.5);
  }
  if (0<rightDistance<30 && leftDistance == 0) {
    folkracer.setAngle(-30);
  }
  if (0<rightDistance<30 && 0<leftDistance<30) {
    folkracer.setSpeed(0.5);
  }
  if (0<rightDistance<30 && leftDistance >30) {
    folkracer.setAngle(-30);      
  }
  if (rightDistance >30 && leftDistance == 0) {
    folkracer.setSpeed(0.5);
  }
  if (rightDistance >30 && leftDistance>0 && leftDistance <30) {
    folkracer.setAngle(30);
  }
  if (rightDistance >30 && leftDistance >30) {
    folkracer.setSpeed(0.5);
  }
  if (rightDistance<leftDistance) {
    folkracer.setAngle(-30);
  }
  if (rightDistance>leftDistance) {
    folkracer.setAngle(30);
  }          
  }
  //Στο σχολείο, να διαβάζετε (στο serial monitor του Arduino IDE) τις αποστάσεις που ανιχνεύει το αυτοκινητάκι ασύρματα μέσω bluetooth, έτσι ώστε να καταλάβετε τι "διαβάζει"
  //το αυτοκινητάκι όταν κινείται στην πίστα!  
  
}

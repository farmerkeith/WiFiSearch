// File SPIFFScli.h to provide a primitive command line interface for SPIFFS file system
// commands ls (list files) and cat (print a file on the serial monitor) and rm (remove a file)
// commands help (list the available commands)
// author farmerkeith on 14 August 2019

class spiffs {

  public:
    String path = "";
    bool rmFlag = 0;

} spiffs;

void SPIFFSY (void) {
  // Y response to rm command question
  if (spiffs.rmFlag == 1) {
    spiffs.rmFlag == 0;
    SPIFFS.remove(spiffs.path);  // removes the path (file)
    Serial.print("\nFile removed "); Serial.print(spiffs.path);
  }
}

void SPIFFSn (void) {
  // n response to rm command question
  spiffs.rmFlag == 0;
  Serial.print("\nAction cancelled");
}

void SPIFFScp(String path) {
  String source = "", destination = "";
  path.trim(); // essential step
  int pathBreak = path.indexOf(' ');
  int pathLength = path.length();
  source = path.substring(0, pathBreak);
  destination = path.substring(pathBreak + 1, pathLength) ;
  Serial.print("\ncp command, parameters=");
  Serial.print(source);
  Serial.print(' ');
  Serial.print(destination);
  Serial.println();

  File f1 = SPIFFS.open(source, "r");  // opens "path1" (file) for reading
  File f2 = SPIFFS.open(destination, "w");  // opens "path" (file) for writing
  if (f1) { // f1 is open to read (source file)
    String s;
    while (f1.position() < f1.size())
    { s = f1.readStringUntil('\n');
      // s.trim();
      f2.println(s); // copy into destination file
    } // end of while (f1.position() < f1.size())
    f1.close(); f2.close();
    Serial.print("\nFile copied from "); Serial.print(source);
    Serial.print(" to "); Serial.print(destination);
  }
} // end of void SPIFFScp

void SPIFFSmv(String path) {
  String source = "", destination = "";
  path.trim(); // essential step
  int pathBreak = path.indexOf(' ');
  int pathLength = path.length();
  source = path.substring(0, pathBreak);
  destination = path.substring(pathBreak + 1, pathLength) ;
  Serial.print("\nmv command, parameters=");
  Serial.print(source);
  Serial.print(' ');
  Serial.print(destination);
  Serial.println();
  byte renameResult = SPIFFS.rename(source, destination);
  Serial.print("\nrenameResult="); Serial.print(renameResult);
} // end of void SPIFFSmv

void SPIFFSaddline(String param) {
  String line = "";
  param.trim();
  String path = "", newLine = "";
  int pathBreak = param.indexOf(' ');
  int paramLength = param.length();
  path = param.substring(0, pathBreak); // param up to first space
  newLine = param.substring(pathBreak + 1, paramLength) ; // after space to end
  Serial.print("\naddline command, parameter=");
  Serial.print(path);
  Serial.print(' ');
  Serial.print(newLine);
  Serial.println();
  if (path == "-re") {
    path = spiffs.path;
    Serial.print(" using ");
    Serial.print(path);
  }
  else spiffs.path = path;
  // Serial.println();
  File f1 = SPIFFS.open(path, "a");  // opens "path" (file) for append
  if (f1) {
    f1.println(newLine); // append new  line
    f1.close();
    Serial.print("\nnew line added to File");
  } else {
    Serial.print("\nFile could not be opened");
  } //  end of else part of if (f1)
} // end of void SPIFFSaddline

void SPIFFSrmline(String path) {
  String line = "";
  path.trim();
  Serial.print("\nrmline command, parameter=");
  Serial.print(path);
  if (path == "-re") {
    path = spiffs.path;
    Serial.print(" using ");
    Serial.print(path);
  }
  else spiffs.path = path;
  // Serial.println();
  int iterator = 0;
  bool allOK = 0;
  String path1;
  do {
    iterator++;
    path1 = path + String(iterator);
    allOK = SPIFFS.exists(path1);
    Serial.print(allOK);
  } while (allOK != 0);
  Serial.print("\ncopy file path is "); Serial.print(path1);
  SPIFFS.rename(path, path1); // rename the file prior to copying row by row
  File f1 = SPIFFS.open(path1, "r");  // opens "path1" (file) for reading
  File f2 = SPIFFS.open(path, "w");  // opens "path" (file) for writing
  if (f1) { // f1 is open to read (source file)
    String s, s1;
    s = f1.readStringUntil('\n');
    while (f1.position() < f1.size())
    { s1 = f1.readStringUntil('\n');
      s.trim();
      f2.println(s); // copy into destination file
      s = s1;
    } // end of while (f1.position() < f1.size())
    f1.close(); f2.close();
    SPIFFS.remove(path1); // delete copy file
    Serial.print("\nFile copied");
  } else {
    Serial.print("\nFile could not be opened");
  } //  end of else part of if (f1)
} // end of void SPIFFSrmline

void SPIFFSrm(String path) {
  String line = "";
  path.trim();
  Serial.print("\nrm command, parameter=");
  Serial.print(path);
  if (path == "-re") {
    path = spiffs.path;
    Serial.print(" using ");
    Serial.print(path);
  }
  else spiffs.path = path;
  Serial.println();
  Serial.println("Are you sure? Y/n");
  spiffs.rmFlag = 1;
} // end of void SPIFFSrm

void SPIFFScat(String path) {
  String line = "";
  path.trim(); // essential step
  Serial.print("\ncat command, parameter=");
  Serial.print(path);
  if (path == "-re") {
    path = spiffs.path;
    Serial.print(" using ");
    Serial.print(path);
  }
  else spiffs.path = path;
  Serial.println();
  File f1 = SPIFFS.open(path, "r");  // opens "path" (file) for reading
  if (!f1) {
    Serial.print("\nFile could not be opened with path="); Serial.print(path);
  }
  else {
    String s;
    while (f1.position() < f1.size())
    { s = f1.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    f1.close();
  }
} // end of void SPIFFScat

void SPIFFSls(void) {
  Serial.print("\nls command received");
  Dir dir = SPIFFS.openDir("");
  // File f;
  while (dir.next()) {
    Serial.print("\n");
    File f = dir.openFile("r");
    Serial.print(dir.fileName());
    Serial.print(" size=");
    Serial.print(dir.fileSize());
  }
/*
  dir.rewind();
  dir.next();
  File f = dir.openFile("r");
  Serial.print("\nf.name()=");
  Serial.print(f.name());
  Serial.print(" f.size()=");
  Serial.print(f.size());
*/
  // f.close();
  FSInfo fileSystem;
  Serial.print("\nfileSystem.totalBytes=");
  Serial.print(fileSystem.totalBytes);
  Serial.print("\nfileSystem.usedBytes=");
  Serial.println(fileSystem.usedBytes);
  Serial.println();

} // end of void SPIFFSls

String formathh_mm_ss(unsigned long ms) {
  int hh = (int)((float)ms / 1000 / 60 / 60);
  String line = "";
  int dd = hh / 24;
  hh = hh % 24;
  if (dd >= 1) {
    line += (String)dd + " day";
    if (dd != 1) line += "s";
  }
  line += " ";
  if  (hh < 10) line += "0";
  line += (String) hh;
  line += ":";
  int mm = (int)((float)ms / 1000 / 60) % 60;
  if  (mm < 10) line += "0";
  line += (String)mm;
  line += ":";
  float ss = (float)ms / 1000;
  ss = ss + (int)ss % 60 - (int)ss;
  if  (ss < 10) line += "0";
  line += ss;
  return line;
}

void SPIFFSgettime(void) {
  Serial.print("\ngettime command received");
  Serial.print("\nTime since last restart is ");
  Serial.println(formathh_mm_ss(millis()));
} // end of void SPIFFSgettime

void SPIFFShelp () {
  // list the available commands
  Serial.println("Available commands");
  Serial.println(" help - prints this message");
  Serial.println(" ls - list of files");
  Serial.println(" cat path - prints the contents of file at path (no quotes required)");
  Serial.println(" cat -re  - prints the contents of file at path in the previous command");
  Serial.println(" rm path - removes the file at path\n -re works with this command");
  Serial.println(" rmline path - removes the last line of the file at path\n -re works with this command");
  Serial.println(" addline path newLine - appends a new line to the file at path\n -re works with this command");
  Serial.println(" cp path1 path2 - copy the file at path1 to path2");
  Serial.println(" mv path1 path2 - rename the file at path1 to path2");
  Serial.println(" gettime - prints the time since the last restart hh:mm:ss.ms");
  Serial.println();
}

void SPIFFScli (void) { // this code runs at intervals ~100ms and starts the command functions as required
  // recognise commands "ls" and "cat path" and "rm path" etc.
  String receivedString = "";
  int characterCount = Serial.available();
  if (characterCount > 0) {
    // Serial.print("\ncharacterCount=");
    // Serial.print(characterCount);
    byte incomingByte[characterCount];
    // read the incoming bytes:
    for (int i = 0; i < characterCount; i++) {
      incomingByte[i] = Serial.read();
      receivedString += char(incomingByte[i]);
    }
    // Serial.print("\nreceivedString length=");
    // Serial.print(receivedString.length());
    // -------------------------------------------------
    if (receivedString.startsWith("ls") &&
        receivedString.length() == 3) {
      // ls command - list the files
      SPIFFSls();
    } else if (receivedString.startsWith("help") &&
               receivedString.length() == 5) {
      // help command - display a help screen
      SPIFFShelp ();
    } else if (receivedString.startsWith("Y") &&
               receivedString.length() == 2) {
      // Y response to rm command
      SPIFFSY ();
    } else if (receivedString.startsWith("n") &&
               receivedString.length() == 2) {
      // n response to rm command
      SPIFFSn ();
    } else if (receivedString.startsWith("cat ")) {
      // cat command - display the file contents
      SPIFFScat (receivedString.substring(4)); // "4" is the starting point
    } else if (receivedString.startsWith("rm ")) {
      // rm command - remove the file
      SPIFFSrm (receivedString.substring(3));
    } else if (receivedString.startsWith("rmline ")) {
      // rmline command - remove the last line of the file
      SPIFFSrmline (receivedString.substring(7));
    } else if (receivedString.startsWith("addline ")) {
      // addline command - adds a new line at the end of the file
      SPIFFSaddline (receivedString.substring(8));
    } else if (receivedString.startsWith("cp ")) {
      // cp command - copies a file
      SPIFFScp (receivedString.substring(3));
    } else if (receivedString.startsWith("mv ")) {
      // mv command - renames a file
      SPIFFSmv (receivedString.substring(3));
    } else if (receivedString.startsWith("gettime") &&
               receivedString.length() >= 7) {

      // gettime command - prints the current time since last restart
      SPIFFSgettime ();
    } else if (receivedString.length() == 1) {
      Serial.println(); // return only
    } else { // string not recognised
      Serial.print("\nI received: ");
      Serial.print(receivedString);
      Serial.print(" incomingByte[0,1]=");
      Serial.print(incomingByte[0]);
      Serial.print(" ");
      Serial.print(incomingByte[1]);
    } // end of else part of if (characterCount > 0)
    // -------------------------------------------------
  } // end of if (characterCount > 0)
} // end of void SPIFFScli

#ifndef TAB_H
#define TAB_H

#include "types.h"

#include <memory.h>

#include "g0/afile.h"
//All definitions to Tablature abstraction

//Most parts now covered under header definition, but refactoring should leave this and hold definitions only but not implementations
//and comments should be used to let know what and where lays

#include "abitarray.h"

int updateDurationWithDetail(byte detail, int base);

//old version of bends used on early moments for
struct BendPointGPOld
{
    ul absolutePosition;
    ul heightPosition;
    byte vibratoFlag;
};

class BendPointsGPOld : public Poly<BendPointGPOld>
{
protected:
    ul bendHeight;
    byte bendType;
public:

    byte getType() { return bendType; }
    void setType(byte newBendType) { bendType = newBendType; }

    ul getHeight() { return bendHeight; }
    void setHeight(ul newBendHeight) { bendHeight=newBendHeight;}

};

struct BendPoint
{
    byte vertical; //:4
    byte horizontal; //:5
    byte vFlag; //:2
};

class BendPoints : public Poly<BendPoint>
{
protected:
    byte bendType;
public:
    byte getType() { return bendType; }
    void setType(byte newBendType) { bendType = newBendType; }

    void insertNewPoint(BendPoint bendPoint);
};



class Note 
{
	//class implements single note, a sound on one string
	public:

    void printToStream(std::ostream &stream);

	enum FretValues
	{
		emptyFret = 63,
		pauseFret = 62,
		ghostFret = 61,
		leegFret = 60
	};
        //check is upper replaced?

    enum NoteState
    {
        normalNote = 0,
        leegNote = 2,
        deadNote = 3,
        leegedNormal = 4,
        leegedLeeg = 6
    };


	Note():
    fret(emptyFret),volume(0),fingering(0),effectsA(0),effectsB(0),graceIsHere(false) {}
	virtual ~Note() {}

    Note &operator=(Note *anotherNote)
    {
        clone(anotherNote);
        return *this;
    }

	protected:
	//[BASE]
	byte fret; //[0-63]; some last values used for special coding {pause, empty, leege note... ghost note(x)}
	byte volume;//[0-64]]-1);  values less than 14 used for ppp pp p m f ff fff - before -is precents for 2 per step
	byte fingering; //store information about finger to play - //fingers {none, L1,L2,L3,L4,L5, R1,R2,R3,R4,R5 +(12)(13)(14)(15)(16) ) - pressure hand for another there
	///are pick effects
		
    byte noteState;//NoteState


	byte effectsA; // inEffects:[slide in up\down; grace; none] outEffects:[side out up\down; legato; bend; ghost; ] [let ring] [palm mute] [vibro]
	byte effectsB; // picking [tap slap pop up down stoks and non] [trill 0 2 4 8]
     
    //moved to set get      
   //short int *bendPoints;
   


	//to set sure  - if possible escape - then do it
	byte GpCompFing1;
	byte GpCompFing2;


    byte stringNumber;

    Note *prevNote;
    Note *nextNote;
	
	public:

    void setNext(Note *nextOne) { nextNote = nextOne; }
    void setPrev(Note *prevOne) { prevNote = prevOne; }

    Note *getNext() { return nextNote; }
    Note *getPrev() { return prevNote; }

    //For GP Compatibility left:
    //Grace note - 4bytes
    bool graceIsHere;
    byte graceNote[4];//compitability with guitar pro requires later migration

	BendPoints bend;
    //[EFFECTS]
    EffectsPack effPack;

	//SET GET operations
    void setStringNumber(byte num) {stringNumber = num;}
    byte getStringNumber() { return stringNumber; }

	void setFret(byte fValue) { fret = fValue; }
	byte getFret() { return fret; }


    int getMidiNote(int tune=0) //get tune from tracks hard chain
    {
        return fret + tune;
    }

    void setState(byte nState) { noteState = nState; }
    void signStateLeeged() { if (noteState<=1) noteState=4; if (noteState==2) noteState=6; }
    byte getState() { return noteState;}
	
	void setVolume(byte vValue) { volume = vValue; }
	byte getVolume() { return volume; }
	
	void setFingering(byte fValue) {fingering = fValue;}
	byte getFingering() { return fingering; }
	
    void setEffect(byte eff);// { effectsA = eff; }
    EffectsPack getEffects();// { return effectsA; }
    void addEffects(EffectsPack &append) { effPack.mergeWith(append); }

	//effects need review
	//MISSING
	//INCLUDING	BEND
			
	void setGPCOMPFing1(byte fValue) { GpCompFing1 = fValue; }
	void setGPCOMPFing2(byte fValue) { GpCompFing2 = fValue; }
	byte getGPCOMPFing1() { return GpCompFing1;}
	byte getGPCOMPFing2() { return GpCompFing2;}
	
	//NO	CHECK	REMINDS	ABOUT	FUTURE	CHANGES
	void setGPCOMPGrace(size_t index, byte gValue) { graceNote[index] = gValue;}
	byte getGPCOMPGrace(size_t index) { return graceNote[index]; }

    void clone(Note *from);
	//pack function
};


//STARTING LARGE REFACTORING OVER THE TAB

class Beat : public Poly<Note*>
{
		
public:
    Beat():effects(0),duration(0),dotted(0),durationDetail(0) {}
    virtual ~Beat()
    {
        for (ul i=0; i < len(); ++i)
                   delete getV(i);
    }
    //usually size reserved

     void printToStream(std::ostream &stream);

	struct SingleChange
	{
		byte changeType;
		ul changeValue;
		byte changeCount;
	};

	//need inner functions for analtics of packing	
	class ChangesList : public Poly<SingleChange>
	{
    public:
        //search functions
	};

	//used for compatibility with guitar pro
	struct GPChordDiagram
	{
		byte header;
		byte sharp;
		byte blank1;
		byte blank2;
		byte blank3;
		
		byte root;
		byte minMaj;
		byte steps9x;
		
		ul bass;
		ul deminush;
		
		byte add;
		char name[20];
		
		byte blank4;
		byte blank5;
		
		byte s5,s9,s11;
		
		ul baseFrets;
		ul frets[7];
		
		byte baresNum;
		byte fretBare[5];
		byte startBare[5];
		byte endBare[5];
		
		byte o1,o3,o5,o7,o9,o11,o13;
		
		byte blank6;
		
		byte fingering[7];
		byte showFing;
	} gpCompChordDiagram;


    struct ChangeTable
    {
       byte newInstr;
       byte newVolume;
       byte newPan;
       byte newChorus;
       byte newReverb;
       byte newPhaser; //6 total + 4
       byte newTremolo;
       ul newTempo; //OR IT IS INT?
       //10
       byte instrDur;
       byte volumeDur;
       byte panDur;
       byte chorusDur;
       byte reverbDur;
       byte phaserDur;
       byte tremoloDur;
       byte tempoDur;

       byte changesTo; //one track or all of them
       byte postFix;
    };

    EffectsPack effPack;

    Beat &operator=(Beat *another)
    {
        clone(another);
        return *this;
    }

protected:

	byte duration; // 2 1 . 2 4 8 16 32 64 [8 values] - 3 bits 
	byte durationDetail; // none, dot, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 [4 bits] //one of 2-15 means empty(together with pause only!)
	//1 bit determines pause
    bool isPaused;

    byte dotted; //0 1 2


	byte effects; //tremolo precene - 1bit, (upstrings, downstrings+x) - 2 bits, bookmark - 1 bit, notice (text or elst) - 1 bit, 
	//then 3 left for change - first bit of those 3 shows that there is change
	//+1 chord diagram
	//+1 reserved
	
	//moved to set get
	//short int *tremoloPoints;
			
	std::string bookmarkName;
	std::string noticeText; 

		  		  
	public:
		
	//pub are part of set and get
	ChangesList changes;
    BendPoints tremolo;



	//SET GET operations
    void setPause(bool pause) {isPaused = pause;}
    bool getPause() { return isPaused; }

    void deleteNote(int string)
    {
        for (ul i = 0; i < len(); ++i)
        {
            if (string == getV(i)->getStringNumber())
            {
                remove(i);

                if (len() == 0)
                    setPause(true);

                return;
            }
        }
    }

    Note *getNote(int string)
    {
        for (ul i = 0; i < len(); ++i)

            if (getV(i)->getStringNumber()==string)
                return getV(i);

        return 0;
    }

    Note *getNoteInstance(int string)
    {
        /*
        for (ul i = 0; i < len(); ++i)

            if (getV(i).getStringNumber()==string)
                return getV(i);
        */

        Note *notePtr = getNote(string);
        if (notePtr)
        {
            Note *theClone = new Note();
            theClone->clone(notePtr);
            return theClone;
        }

        Note *emptyNote = new Note;
        emptyNote->setFret(255);
        return emptyNote;
    }

    void setFret(byte fret, int string)
    {

        if (len() == 0)
        {
            //paused
            Note *newNote = new Note();
            newNote->setFret(fret);
            newNote->setStringNumber(string);
            newNote->setState(0);

            //DEFAULT NOTE VALUES??
            add(newNote);

            setPause(false);
            return;
        }

        for (ul i = 0; i < len(); ++i)
        {
            if (getV(i)->getStringNumber()==string)
            {
                getV(i)->setFret(fret);
                return; //function done
            }

            if (getV(i)->getStringNumber() > string)
            {
                Note *newNote=new Note();
                newNote->setFret(fret);
                newNote->setStringNumber(string);
                newNote->setState(0);


                insertBefore(newNote,i);
                return;
            }

        }


        int lastStringN = getV(len()-1)->getStringNumber();
        if (lastStringN < string)
        {
            Note *newNote=new Note();
            newNote->setFret(fret);
            newNote->setStringNumber(string);
            newNote->setState(0);

            //DEFAULT NOTE VALUES??
            add(newNote);
            return;
        }
        //we got here - that means we need insert fret
    }


    byte getFret(int string)
    {
        if (len() == 0)
            return 255;

        for (ul i = 0; i < len(); ++i)
            if (getV(i)->getStringNumber()==string)
            {
                byte fretValue = getV(i)->getFret();
                return fretValue; //function done
            }

        return 255;
    }

	void setDuration(byte dValue) { duration = dValue; }	
	void setDurationDetail(byte dValue) {	durationDetail = dValue; }
    void setDotted(byte dottedValue) { dotted = dottedValue; }
	
	byte getDuration() { return duration;}
	byte getDurationDetail () { return durationDetail; }
    byte getDotted() { return dotted; }

    void setEffects(byte eValue);// { effects = eValue; }
    EffectsPack getEffects();// { return effects; }

	//TREMOLO missing
	
	//BOOKMARK - goes  for future
	
    void setGPCOMPChordDiagram(char *area) { memcpy(&gpCompChordDiagram,area,sizeof(gpCompChordDiagram)); }
    bool getGPCOMPChordDiagram(char *to) { memcpy(to,&gpCompChordDiagram,sizeof(gpCompChordDiagram)); return true; }

	void setGPCOMPText(std::string &value) { noticeText = value; }	
	void getGPCOMPText(std::string &value) { value = noticeText; }
	
    //pack function

    void clone(Beat *from);
};

class Bar : public Poly<Beat*>
{
	
public:
    Bar() {
        flush();
    }
    virtual ~Bar()
    {
        for (ul i=0; i < len(); ++i)
                   delete getV(i);
    }

    void printToStream(std::ostream &stream);

    void flush()
    {
        signatureNum = signatureDenum = 0;
        repeat = repeatTimes = altRepeat = 0;
        markerColor = 0;
        completeStatus = 0;
    }

    Bar &operator=(Bar *another)
    {
        clone(another);
        return *this;
    }

    virtual void add(Beat *&val)
    {
        if (val)
        {
            val->setParent(this);
            Poly<Beat*>::add(val);
        }
    }

    virtual void insertBefore(Beat* &val, int index=0)
    {
        if (val)
        {
            val->setParent(this);
            Poly<Beat*>::insertBefore(val,index);
        }
    }


protected:

	byte signatureNum;
	byte signatureDenum;
	
	byte repeat;//reprease options
    byte repeatTimes;
    byte altRepeat;
	
	//GP comp - marker, tonality
	byte gpCompTonality;
	std::string markerText;
	ul markerColor; //white byte == 1 if empty

    byte completeStatus;
    short completeAbs;
    ul completeIndex;

	public:
	//SET GET operations
	
	void setSignNum(byte num) { signatureNum = num; }
	void setSignDenum(byte denum) { signatureDenum = denum; }
	
	byte getSignNum() { return signatureNum; }
	byte getSignDenum() { return signatureDenum; }

    //!completeStatus !!! that should go private and opt
    void countUsedSigns(byte &numGet, byte &denumGet);

    byte getCompleteStatus();
    double getCompleteAbs();
    ul   getCompleteIndex();
	
    void setRepeat(byte rValue, byte times=0)
    {
        if (rValue == 0) repeat = 0;
        else repeat |= rValue;
        if(times) repeatTimes=times;
    }
	byte getRepeat() { return repeat; }
    byte getRepeatTimes() { return repeatTimes; }

    void setAltRepeat(byte number) { altRepeat = number; }
    byte getAltRepeat() { return altRepeat; }
	
	void setGPCOMPTonality(byte tValue) { gpCompTonality = tValue; }
	byte getGPCOMPTonality() { return gpCompTonality; }
	
	void setGPCOMPMarker(std::string &text, ul color) { markerText = text; markerColor = color; }
	void getGPCOMPMarker(std::string &text, ul &color) { text = markerText; color = markerColor; } 
		
	//pack function
     void clone(Bar *from);
};

class Aplicature : public Poly<std::string> //then pack nice to ul
{
public:
    //interpritation as text 1:0;2:3; [:effect:]string:scale[:effect];
    //digits and signs of effects

    void setFromString(std::string str);
    //to Melody

    std::string to_s();
};

class GuitarTuning
{
	byte stringsAmount;
    byte tunes[10]; //as a maximum, mind later is it usefull to try set more or less by container?
    //set it to byte - in fact int would be 128 values of std midi - next could be used as quatones

	public:
	
	void setStringsAmount(byte amount) { stringsAmount = amount; }
	byte getStringsAmount() { return stringsAmount; }
    void setTune(byte index, byte value) { if (index <= 10) tunes[index] = value; } //(index >= 0) &&
    byte getTune(byte index) { if (index <= 10) return tunes[index]; return 0; }
};


class PolyBar : public Poly<Bar*>
{
  public:
    PolyBar()
    {
    }
    virtual ~PolyBar()
    {
    }
};

class Track : public Poly<Bar*>
{
protected:
    //Aplicature applic;
    //void fullFillAplicature();
    ///void updateAplicature(); //list of changes
public:

    Track():timeLoop(),drums(false),status(0){ GpCompInts[3]=24; } //REFACT GCOMP
    virtual ~Track()
    {
        for (ul i=0; i < len(); ++i)
                   delete getV(i);
    }

    void printToStream(std::ostream &stream);

    Poly<Bar*> timeLoop; //PolyBar //REFACT access
    std::vector<ul> timeLoopIndexStore;

    Track &operator=(Track another)
    {
        //clone(another);
        return *this;
    }

    virtual void add(Bar *&val)
    {
        if (val)
        {
            val->setParent(this);

            Poly<Bar*>::add(val);
        }
    }

    virtual void insertBefore(Bar* &val, int index=0)
    {
        if (val)
        {
            val->setParent(this);
        Poly<Bar*>::insertBefore(val,index);
        }
    }

protected:
	std::string name;
	ul instrument;
	ul color;
	byte pan; //or int??
	byte volume;

    bool drums;
	//?own temp bpm
	//GpComp - Port,Channel,ChannelE,Capo
	ul GpCompInts[4];

    ul beatsAmount;

    byte status; //0 - none 1 - mute 2 - soloe

	public:

//pub classes are part of set get operations
	GuitarTuning tuning;

    ul connectBars();
    ul connectBeats();
    ul connectNotes(); //for let ring
    ul connectTimeLoop();

    void connectAll();


    void pushReprise(Bar *beginRepeat, Bar *endRepeat,
                     Bar *preTail, Bar *tailBegin, Bar *tailEnd, ul beginIndex, ul endIndex,
                     ul preTailIndex=0, ul tailBeginIndex=0, ul tailEndIndex=0);


//SET GET operations
	
	void setName( std::string &nValue) { name = nValue; }
    std::string getName() { return name; } //or return?
	
	void setInstrument(ul iValue) { instrument = iValue;}
	ul getInstrument() { return instrument; }
	
	void setColor(ul cValue) { color = cValue; }
	ul getColor() { return color; }
	
    void setPan(byte pValue) { pan = pValue; }
	byte getPan() { return pan; }
	
	void setVolume(byte vValue) { volume = vValue; }
	byte getVolume() { return volume; }
	
	//skip checks yet
	void setGPCOMPInts(size_t index, ul value) { GpCompInts[index] = value; }
	ul getGPCOMPInts(size_t index) { return GpCompInts[index]; }
	
    void setDrums(bool newDrums) {
        drums = newDrums;
        if (drums) GpCompInts[3]=99; //refact
    }

    bool isDrums() { return drums; }
	//pack function	

    byte getStatus() { return status; } //refact name
    void setStatus(byte newStat) { status = newStat; }
};

//here would lay Guitar Pro header comptibilator
struct MidiChannelInfo
{
    unsigned int instrument; //refact types

	byte volume;
	byte balance;
	byte chorus;
	byte reverb;

	byte phaser;
	byte tremolo;
	byte blank1;
	byte blank2;
};


struct VariableString
{
	byte lineType;
	std::string name;
	
	std::list<std::string> v;
	
	VariableString(std::string stringName, std::string stringValue) : lineType(0), name(stringName) { v.push_back(stringValue); }
	VariableString(std::string stringName, std::list<std::string> stringsValues) : lineType(1), name(stringName) { v = stringsValues; }
};

class VariableStrings : public Poly<VariableString>
{
	//search options
};


struct TimeLineKnot
{
    byte type;
    int value;

    TimeLineKnot(byte newType, int newValue):type(newType),value(newValue){}
};


class Tab : public Poly<Track*>
{
public:
    //cover it later
    std::vector<TimeLineKnot> timeLine;

    //Tab(); 
    virtual ~Tab()
    {
        for (ul i=0; i < len(); ++i)
                   delete getV(i);
    }

    void printToStream(std::ostream &stream);

    void createTimeLine(ul shiftTheCursor=0);

    byte getBPMStatusOnBar(ul barN);
    int getBpmOnBar(ul barN);

    void connectTracks()
    {
        for (ul i = 0; i < len(); ++i)
        getV(i)->connectAll();

        createTimeLine();
    }

    void postGTP()
    {
        for (ul i = 0; i < len(); ++i)
        {
            ul port = getV(i)->getGPCOMPInts(0);
            ul chan = getV(i)->getGPCOMPInts(1);
            ul ind = (chan-1) + (port-1)*16;
            if (ind < 70)
            {
                int instr = GpCompMidiChannels[ind].instrument;
                byte pan = GpCompMidiChannels[ind].balance;
                byte vol = GpCompMidiChannels[ind].volume;
                Track *t=getV(i);
                t->setInstrument(instr);
                t->setPan(pan);
                t->setVolume(vol);
            }
         }
    }

    Tab &operator=(Tab another)
    {
        //lone(another);
        return *this;
    }


protected:	
    int bpmTemp;
	//polyrithmm and own temp for track flags
	//version control flag
	//compitibility flag

	std::string origin; //glink - short link determines were from file came (from guitarmy network) az09AZ style
	
		
	//better class for header
	
	byte GpCompSignKey;
	byte GpCompSignOctave;
	byte GpCompTripletFeel;



	public:
//public classes live before  set get operations as them part of them

		VariableStrings variableInforation;
        MidiChannelInfo GpCompMidiChannels[64];

public:
//SET GET operations
        int getBPM() { return bpmTemp; }
        void setBPM(int newBPM) { bpmTemp = newBPM; }
	
	
//pack function				
};


//some usefull

int translateDenum(byte den);

int translaeDuration(byte dur);


////////////////////Pack reference - used only for GMY format///////////////////////////

	//pack guide:
	//fret - 6 bit; volume - 6 bit; fingering - 4 bit; = 2bytes
	//...on another mind 3 bits could be used as flags - effects,fingering+else
	//or volume could be packed in smaller distance [0-8] or [0-16]
    //pack effects into one more byte, and if bend is present, then:
   
   // semisemisemi tone (1/8) semisemi tone (1/4) semi tone (1/2) tone (1) double tone (2) - 5 bits for presence of each
   //duration of point - (double, single, half, 4, 8, 16, 32, 64) - 8 bits
   //+ 1 bit flag of last point
   //+ 1 bit flag of not finishing bend
   //+ 1 bit flag of continued bend

	//So in normal mode minimal note will have 3 bytes; another mode - extra packing
	//0-31 fret with secial coding (limits amount of frets - but always more then 24)
	//8 values for volume - forte piano stype - 3 bits 
	
	//next byte is really hard but helps compression - 
	//if hierest bit is == 1 then effects follow
	//if hierest bit it == 0 then 
						// if next bit == 1 then we have note of same volume - and fret stored inside;
						//next note comes in same way
	//another secret 0-31 for fret and 3 bits for [effects precence1 bit] 2 reserved - volume as duration will store in Beat
	//change precence - in Beat will show that duration changin, or volume is changing - for extra high packing
	//NO	MO	MINDS	ANOUT	HIGH	PACK. main ideas stored here now




#endif // TAB_H

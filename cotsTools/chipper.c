/* (S)Chip-48 Assembler V2.11 by Christian Egeberg 2/11-'90 .. 20/8-'91 */

#define CopyRight "(S)Chip-48 Assembler V2.11 by Christian Egeberg 20/8-\'91"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Modify macro below to force Ansi or Kernighan-Ritchie prototypes */
#define ANSI ( defined( c_plusplus ) || defined( __cplusplus ) || __STDC__ )
/* Modify macro below to change procedure storage class */
#define PROC

#define False     ( char )0
#define True      ( char )~0
#define UnDefined 0L
#define Defined   ~0L

#define StartAddress 0x200L
#define StopAddress  0xfffL
#define CheckMagic   0x1081L
#define WordMask     0xffffL
#define AddrMask     0xfffL
#define ByteMask     0xffL
#define NibbleMask   0xfL

#define SpaceLength     32768 
#define LineLength      255
#define ParamLength     127
#define SymbolLength    32
#define ListLength      32
#define StackLength     32  
#define MaxBaseLength   16
#define BinHpHeadLength 13
#define FailExitCode    1
#define SuccessExitCode 0

#define NullChar      '\0'
#define BellChar      '\7'
#define SpaceChar     ' '
#define SeparatorChar ','
#define RemarkChar    ';'
#define SymbolChar    '_'
#define LabelChar     ':'
#define EqualChar     '='
#define TextChar      '\''
#define AddressChar   '\77'
#define HexChar       '#'
#define BinChar       '$'
#define OctChar       '@'
#define AscChar       '\"'
#define StartChar     '('
#define StopChar      ')'
#define PlusChar      '+'
#define MinusChar     '-'
#define NotChar       '~'
#define PowerChar     '!'
#define ShlChar       '<'
#define ShrChar       '>'
#define MulChar       '*'
#define FracChar      '/'
#define AndChar       '&'
#define OrChar        '|'
#define XorChar       '^'
#define DivChar       '\\'
#define ModChar       '%'

#define WrongToken    0
#define EqualToken    1
#define AddToken      2
#define AlignToken    3
#define AndToken      4
#define CallToken     5
#define ClsToken      6
#define DaToken       7
#define DbToken       8
#define DefineToken   9
#define DrwToken     10
#define DsToken      11
#define DwToken      12
#define ElseToken    13
#define EndToken     14
#define EndifToken   15
#define EquToken     16
#define ExitToken    17
#define HighToken    18
#define IfdefToken   19
#define IfundToken   20
#define IncludeToken 21
#define JpToken      22
#define LdToken      23
#define LowToken     24
#define OptionToken  25
#define OrToken      26
#define OrgToken     27
#define RetToken     28
#define RndToken     29
#define ScdToken     30
#define SclToken     31
#define ScrToken     32
#define SeToken      33
#define ShlToken     34
#define ShrToken     35
#define SknpToken    36
#define SkpToken     37
#define SneToken     38
#define SubToken     39
#define SubnToken    40
#define SysToken     41
#define UndefToken   42
#define UsedToken    43
#define XorToken     44
#define XrefToken    45
#define LastToken    46

#define BadReg   0
#define BReg     1
#define DtReg    2
#define FReg     3
#define HfReg    4
#define IReg     5
#define KReg     6
#define LfReg    7
#define RReg     8
#define StReg    9
#define V0Reg   10
#define V1Reg   11
#define V2Reg   12
#define V3Reg   13
#define V4Reg   14
#define V5Reg   15
#define V6Reg   16
#define V7Reg   17
#define V8Reg   18
#define V9Reg   19
#define VaReg   20
#define VbReg   21
#define VcReg   22
#define VdReg   23
#define VeReg   24
#define VfReg   25
#define IiReg   26
#define LastReg 27

#define AlignOnDefault  Defined
#define Chip8Default    UnDefined
#define Chip48Default   Defined
#define Super10Default  UnDefined
#define Super11Default  UnDefined
#define UsedYesDefault  UnDefined
#define UsedOnDefault   UnDefined
#define XrefYesDefault  Defined
#define XrefOnDefault   Defined
#define HpHeadDefault   Defined
#define HpAscDefault    UnDefined

typedef char CharSet[ 0x100 ];
typedef char LineString[ LineLength + 1 ];
typedef char ParamString[ ParamLength + 1 ];
typedef char SymbolString[ SymbolLength + 1 ];
typedef struct SpaceRecord {
  unsigned int Size;
  char *Start;
  char *Point;
  char *Index;
  unsigned int Request;
} SpaceRecord;
typedef struct LocRecord *LocPointer;
typedef struct LocRecord {
  unsigned int Line; 
  char *Name;
  char *Text;
  char Xref;
  long int Addr;
} LocRecord;
typedef struct ReferRecord *ReferPointer;
typedef struct ReferRecord {
  LocRecord Loc;
  ReferPointer Next;
} ReferRecord;
typedef struct ParamRecord *ParamPointer;
typedef struct ParamRecord {
  char *Param;
  ParamPointer Next;
} ParamRecord;
typedef struct SymbolRecord *SymbolPointer;
typedef struct SymbolRecord {
  LocRecord Loc;
  long int Value;
  char *Symbol;
  char *Expr;
  ReferPointer Refer;
  SymbolPointer Left;
  SymbolPointer Right;
} SymbolRecord;
typedef struct InstRecord *InstPointer;
typedef struct InstRecord { 
  LocRecord Loc;
  unsigned int Token;
  unsigned int Count;
  ParamPointer Params;
  InstPointer Next;
} InstRecord;

static char InExtDefault[] = ".chp";
static char ListExtDefault[] = ".lst";
static char DefFileName[] = ".";
static char StdFileName[] = "-";
static char StdInFileName[] = "stdin";
static char StdOutFileName[] = "stdout";
static char BinHpHeadText[] = "HPHP48-C";
static char AscHpHeadText[] = "%%HP: T(3)A(R)F(.);\n";

static char RunErrorMessage[] = "Fatal error: ";
static char RunWarningMessage[] = "Warning: ";
static char WarningNumMessage[] = "Total number of warnings: ";
static char EndOfFileMessage[] = "; End of file";
static char NoTargetError[] = "Usage is.. chipper Target [Source] [List]";
static char NoSourceError[] = "No source file found";
static char FileAccessError[] = "Unable to open file";
static char FileWriteError[] = "File or pipe output failed";
static char AllocationError[] = "Unable to allocate more memory";
static char SpaceAssertError[] = "Internal memory allocation mismatch";
static char NestedIfdefError[] = "Too many nested conditions";
static char BoundsError[] = "Outside legal address range";
static char ParamCountWarning[] = "Incorrect number of parameters";
static char DualSymbolWarning[] = "No directive recognized";
static char NoOptionWarning[] = "Option not recognized";
static char MissingSymbolWarning[] = "No symbol name specified";
static char NoSymbolWarning[] = "Not a defined symbol";
static char UnusedSymbolWarning[] = "Unused symbol detected";
static char CopySymbolWarning[] = "Existing symbol redefined";
static char UndefinedWarning[] = "Unable to evaluate parameter";
static char RangeWarning[] = "Parameter out of range";
static char NoRegisterWarning[] = "No register recognized";
static char BadRegisterWarning[] = "Illegal register specified";
static char NeedsChip8Warning[] = "Chip-8 spesific directive";
static char NeedsChip48Warning[] = "Chip-48 spesific directive";
static char NeedsSuper10Warning[] = "Super Chip-48 V1.0.. spesific directive";
static char NeedsSuper11Warning[] = "Super Chip-48 V1.1.. spesific directive";
static char MissingIfdefWarning[] = "No previous condition found";
static char CountDefineWarning[] = "Unbalanced condition matching in file";
static char InternalWarning[] = "Internal data structure mismatch";

static char AlignOnName[] = "ALIGNON";
static char Chip8Name[] = "CHIP8";
static char Chip48Name[] = "CHIP48";
static char Super10Name[] = "SCHIP10";
static char Super11Name[] = "SCHIP11";
static char UsedYesName[] = "USEDYES";
static char UsedOnName[] = "USEDON";
static char XrefYesName[] = "XREFYES";
static char XrefOnName[] = "XREFON";
static char HpHeadName[] = "HPHEAD";
static char HpAscName[] = "HPASC";
static char HpBinName[] = "HPBIN";
static char BinaryName[] = "BINARY";
static char StringName[] = "STRING";

static char OptionYesName[] = "YES";
static char OptionNoName[] = "NO";
static char OptionOnName[] = "ON";
static char OptionOffName[] = "OFF";

static SymbolString TokenText[ LastToken ] = 
  { ",,,,,",   "=",       "ADD",     "ALIGN",   "AND",
    "CALL",    "CLS",     "DA",      "DB",      "DEFINE",
    "DRW",     "DS",      "DW",      "ELSE",    "END",     
    "ENDIF",   "EQU",     "EXIT",    "HIGH",    "IFDEF",   
    "IFUND",   "INCLUDE", "JP",      "LD",      "LOW",     
    "OPTION",  "OR",      "ORG",     "RET",     "RND",     
    "SCD",     "SCL",     "SCR",     "SE",      "SHL",     
    "SHR",     "SKNP",    "SKP",     "SNE",     "SUB",     
    "SUBN",    "SYS",     "UNDEF",   "USED",    "XOR",     
    "XREF"                                                };

static SymbolString RegisterText[ LastReg ] =
  { ",,,", "B",   "DT",  "F",   "HF",  "I",   "K",   "LF",
    "R",   "ST",  "V0",  "V1",  "V2",  "V3",  "V4",  "V5",
    "V6",  "V7",  "V8",  "V9",  "VA",  "VB",  "VC",  "VD",
    "VE",  "VF",  "[I]"                                    };

static SpaceRecord Space = { SpaceLength, NULL, NULL, NULL, 0 };

static char StackStart[ StackLength + 1 ];
static char *StackPoint = StackStart;

static CharSet Operators;
static char StartOperator[] = { StartChar, NullChar };
static char StopOperator[] = { StopChar, NullChar };
static char UnaryOperator[] = { PlusChar, MinusChar, NotChar, NullChar };
static char PowerOperator[] = { PowerChar, ShlChar, ShrChar, NullChar };
static char MulDivOperator[] = { MulChar, FracChar, NullChar };
static char PlusMinusOperator[] = { PlusChar, MinusChar, NullChar };
static char BitWiseOperator[] = { AndChar, OrChar, XorChar, NullChar };
static char DivModOperator[] = { DivChar, ModChar, NullChar };
static ParamString DigitText = "0123456789ABCDEF.";
static char NullString[] = "\0";
static LineString Separator = "";

static long int DefinedValue = Defined;
static long int DummyValue = UnDefined;
static long int *AlignOnCond = &DummyValue;
static long int *Chip8Cond = &DummyValue;
static long int *Chip48Cond = &DummyValue;
static long int *Super10Cond = &DummyValue;
static long int *Super11Cond = &DummyValue;
static long int *UsedYesCond = &DummyValue;
static long int *UsedOnCond = &DummyValue;
static long int *XrefYesCond = &DummyValue;
static long int *XrefOnCond = &DummyValue;
static long int *HpHeadCond = &DummyValue;
static long int *HpAscCond = &DummyValue;

static InstPointer Instructions = NULL;
static SymbolPointer Directives = NULL;
static SymbolPointer Registers = NULL;
static SymbolPointer Symbols= NULL;
static SymbolPointer Conditions= NULL;
static InstPointer InstPoint = NULL;
static SymbolPointer SymbPoint = NULL;
static SymbolPointer CurrentSymbol = NULL;

static long int FinalAddress = StartAddress;
static LocRecord NullLocation = { 0, NullString, NullString, False, 0L };
static LocRecord UsedLocation = { 0, NullString, NullString, True, 0L };
static LocRecord Location = { 0, NullString, NullString, False, StartAddress };
static unsigned int WarningCount = 0;

static FILE *InFile = NULL;
static FILE *OutFile = NULL;
static FILE *ListFile = NULL;
static LineString InFileName = "";
static LineString OutFileName = "";
static LineString ListFileName = "";

static unsigned char Memory[ StopAddress - StartAddress + 1 ];

#if ANSI
  static PROC char ResolveDivMod( char *Symbol, char **Param,
    long int *Value, SymbolPointer Head, LocPointer Loc );
  static PROC void DecodeFile( char *FileName, InstPointer *Head,
    SymbolPointer Dir, SymbolPointer *Symb, SymbolPointer *Cond );
#else
  static PROC char ResolveDivMod();
  static PROC void DecodeFile();
#endif

#if ANSI
  static PROC void RunError( char AbortFlag, char *Message )
#else
  static PROC void RunError( AbortFlag, Message )
  char AbortFlag;
  char *Message;
#endif
{
  if( AbortFlag ) {
    fprintf( stderr, "%c%s\n%s%s\n", BellChar, Separator, RunErrorMessage,
      Message );
    if( ListFile )
      fprintf( ListFile, "%s\n%s%s\n", Separator, RunErrorMessage, Message );
  } else {
    fprintf( stderr, "%s\n%s%s\n", Separator, RunWarningMessage, Message );
    if( ListFile )
      fprintf( ListFile, "%s\n%s%s\n", Separator, RunWarningMessage, Message );
  }
  if( Location.Line ) {
    fprintf( stderr, "Current file %s line %u\n%s\n", Location.Name,
      Location.Line, Location.Text );
    if( ListFile ) 
      fprintf( ListFile, "Current file %s line %u\n%s\n", Location.Name,
        Location.Line, Location.Text );
  }
  if( InstPoint ) {
    fprintf( stderr, "Associated file %s line %u\n%s\n",
      InstPoint->Loc.Name, InstPoint->Loc.Line, InstPoint->Loc.Text );
    if( ListFile ) 
      fprintf( ListFile, "Associated file %s line %u\n%s\n",
        InstPoint->Loc.Name, InstPoint->Loc.Line, InstPoint->Loc.Text );
  }
  if( SymbPoint ) {
    fprintf( stderr, "Symbol %s file %s line %u\n%s\n", SymbPoint->Symbol,
      SymbPoint->Loc.Name, SymbPoint->Loc.Line, SymbPoint->Loc.Text );
    if( ListFile ) 
      fprintf( ListFile, "Symbol %s file %s line %u\n%s\n", SymbPoint->Symbol,
        SymbPoint->Loc.Name, SymbPoint->Loc.Line, SymbPoint->Loc.Text );
  }
  fprintf( stderr, "%s\n", Separator );
  if( ListFile )
    fprintf( ListFile, "%s\n", Separator );
  if( AbortFlag ) 
    exit( FailExitCode );
    /* exit function should close all files */
  else
    WarningCount++;
}

#if ANSI
  static PROC char *NumberString( char *Result, long int Value,
    unsigned int Base, unsigned int Count )
#else
  static PROC char *NumberString( Result, Value, Base, Count )
  char *Result;
  long int Value;
  unsigned int Base;
  unsigned int Count;
#endif
{
  char *Digit;

  Digit = Result + Count;
  *( Digit-- ) = NullChar;
  while(( Digit >= Result ) && ( Value > 0L )) {
    *( Digit-- ) = DigitText[ Value % Base ];
    Value /= Base;
  }
  while( Digit >= Result ) 
    *( Digit-- ) = '0';
  return( Result );
}

#if ANSI
  static PROC void ListInstruction( long int Address, unsigned int Count,
    InstPointer Inst )
#else
  static PROC void ListInstruction( Address, Count, Inst )
  long int Address;
  unsigned int Count;
  InstPointer Inst;
#endif
{
  unsigned int This;
  SymbolString HexText;

  if( fprintf( ListFile, "%s\n%s(%u).. %s: ", Inst->Loc.Text, Inst->Loc.Name,
    Inst->Loc.Line, NumberString( HexText, Address, 16, 3 )) <= 0 )
      RunError( True, FileWriteError );
  for( This = 0; This < Count; This++ )
    if( fprintf( ListFile, NumberString( HexText, ( long int )
      Memory[ Address + This - StartAddress ], 16, 2 )) <= 0 )
        RunError( True, FileWriteError );
  fprintf( ListFile, "\n" );
}

#if ANSI
  static PROC void ListReference( ReferPointer Head )
#else
  static PROC void ListReference( Head )
  ReferPointer Head;
#endif
{
  for( ; Head; Head = Head->Next )
    if( Head->Loc.Line && Head->Loc.Xref )
      if( fprintf( ListFile, "  %s(%u)\n", Head->Loc.Name,
        Head->Loc.Line ) <= 0 )
          RunError( True, FileWriteError );
}

#if ANSI
  static PROC void ListSymbols( SymbolPointer Head )
#else
  static PROC void ListSymbols( Head )
  SymbolPointer Head;
#endif
{
  SymbolString HexText;

  if( Head ) {
    if( Head->Left )
      ListSymbols( Head->Left );
    SymbPoint = Head;
    if( !( *UsedYesCond ) && Head->Loc.Line && !( Head->Refer ))
      RunError( False, UnusedSymbolWarning );
    if( Head->Expr )
      strcpy( HexText, "UND" );
    else
      NumberString( HexText, Head->Value, 16, 3 );
    if( fprintf( ListFile, "%s %s %s(%u)\n", HexText, Head->Symbol,
      Head->Loc.Name, Head->Loc.Line ) <= 0 )
        RunError( True, FileWriteError );
    if( *XrefYesCond )
      ListReference( Head->Refer );
    SymbPoint = NULL;
    if( Head->Right )
      ListSymbols( Head->Right );
  }
}

#if ANSI
  static PROC void ListDefines( SymbolPointer Head )
#else
  static PROC void ListDefines( Head )
  SymbolPointer Head;
#endif
{
  if( Head ) {
    if( Head->Left )
      ListDefines( Head->Left );
    if( Head->Value ) {
      if( fprintf( ListFile, "DEF %s\n", Head->Symbol ) <= 0 )
        RunError( True, FileWriteError );
    } else 
      if( fprintf( ListFile, "UND %s\n", Head->Symbol ) <= 0 )
        RunError( True, FileWriteError );
    if( Head->Right )
      ListDefines( Head->Right );
  }
}

#if ANSI
  static PROC void ListWarnings( void )
#else
  static PROC void ListWarnings()
#endif
{
  if( fprintf( stderr, "%s%u\n", WarningNumMessage, WarningCount ) <= 0 )
    RunError( True, FileWriteError );
  if( fprintf( ListFile, "%s%u\n", WarningNumMessage, WarningCount ) <= 0 )
    RunError( True, FileWriteError );
}

#if ANSI
  static PROC char *AssertSpace( SpaceRecord *Chain, unsigned int Size )
#else
  static PROC char *AssertSpace( Chain, Size )
  SpaceRecord *Chain;
  unsigned int Size;
#endif
{
  if( Chain ) {
    if( Chain->Request )
      RunError( True, SpaceAssertError );
    if( Size + sizeof( char * ) > Chain->Size )
      RunError( True, SpaceAssertError );
    if( !( Chain->Start )) {
      Chain->Start = ( char * )malloc( Chain->Size );
      if( !( Chain->Start ))
        RunError( True, AllocationError );
      Chain->Point = NULL;
    }
    if( !( Chain->Point )) {
      Chain->Point = Chain->Start;
      Chain->Index = NULL;
    }
    if( !( Chain->Index )) {
      *( char ** )Chain->Point = NULL;
      Chain->Index = Chain->Point + sizeof( char * );
    }
    if( Chain->Index + Size > Chain->Point + Chain->Size ) {
      char *This;

      This = ( char * )malloc( Chain->Size );
      if( !This )
        RunError( True, AllocationError );
      *( char ** )Chain->Point = This;
      Chain->Point = This;
      *( char ** )This = NULL;
      Chain->Index = This + sizeof( char * );
    }
  } else
    RunError( True, SpaceAssertError );
  Chain->Request = Size;
  return( Chain->Index );
}

#if ANSI
  static PROC void ClaimSpace( SpaceRecord *Chain, unsigned int Size )
#else
  static PROC void ClaimSpace( Chain, Size )
  SpaceRecord *Chain;
  unsigned int Size;
#endif
{
  if( Chain ) {
    if( !( Chain->Request ))
      RunError( True, SpaceAssertError );
    if( Size > Chain->Request )
      RunError( True, SpaceAssertError );
    Chain->Index += Size;
    Chain->Request = 0;
  } else
    RunError( True, SpaceAssertError );
}

#if ANSI
  static PROC void ReleaseSpace( SpaceRecord *Chain )
#else
  static PROC void ReleaseSpace( Chain )
  SpaceRecord *Chain;
#endif
{
  char *This;
  char *Next;

  if( Chain ) {
    if( Chain->Request )
      RunError( True, SpaceAssertError );
    for( This = Chain->Start; This; ) {
      Next = *( char ** )This;
      free( This );
      This = Next;
    }
    Chain->Start = NULL;
    Chain->Point = NULL;
    Chain->Index = NULL;
    Chain->Request = 0;
  } else
    RunError( True, SpaceAssertError );
}

#if ANSI
  static PROC char OpenFiles( int argc, char *argv[] )
#else
  static PROC char OpenFiles( argc, argv )
  int argc;
  char *argv[]; 
#endif
{
  if( argc >= 2 ) {
    strcpy( OutFileName, argv[ 1 ]);
    if( argc >= 3 )
      strcpy( InFileName, argv[ 2 ]);
    else {
      strcpy( InFileName, OutFileName );
      strcat( InFileName, InExtDefault );
    }
    if( !strcmp( InFileName, DefFileName )) {
      strcpy( InFileName, OutFileName );
      strcat( InFileName, InExtDefault );
    }
    if( !strcmp( InFileName, StdFileName )) 
      strcpy( InFileName, StdInFileName );
    if( argc >= 4 ) 
      strcpy( ListFileName, argv[ 3 ]);
    else {
      strcpy( ListFileName, OutFileName );
      strcat( ListFileName, ListExtDefault );
    }
    if( !strcmp( ListFileName, DefFileName )) {
      strcpy( ListFileName, OutFileName );
      strcat( ListFileName, ListExtDefault );
    }
    if( !strcmp( ListFileName, StdFileName )) 
      strcpy( ListFileName, StdOutFileName );
  } else
    RunError( True, NoTargetError );
  if( fprintf( stderr, "TargetFile: %s\n", OutFileName ) <= 0 )
    RunError( True, FileWriteError );
  OutFile = fopen( OutFileName, "w+b" );
  if( !OutFile )
    RunError( True, FileAccessError );
  if( fprintf( stderr, "SourceFile: %s\n", InFileName ) <= 0 )
    RunError( True, FileWriteError );
  if( strcmp( InFileName, StdInFileName ))
    InFile = fopen( InFileName, "r" );
  else 
    InFile = stdin;
  if( !InFile )
    RunError( True, NoSourceError );
  if( fprintf( stderr, "ListFile: %s\n", ListFileName ) <= 0 )
    RunError( True, FileWriteError );
  if( strcmp( ListFileName, StdOutFileName )) 
    ListFile = fopen( ListFileName, "w" );
  else
    ListFile = stdout;
  if( !ListFile )
    RunError( True, FileAccessError );
  fprintf( stderr, "\n" );
  if( fprintf( ListFile,
    "%s\n\nTargetFile: %s\nSourceFile: %s\nListFile: %s\n\n", CopyRight,
    OutFileName, InFileName, ListFileName ) <= 0 )
      RunError( True, FileWriteError );
  return( True );
}

#if ANSI
  static PROC void DefineReference( LocPointer Loc, ReferPointer *Head )
#else
  static PROC void DefineReference( Loc, Head )
  LocPointer Loc;
  ReferPointer *Head;
#endif
{
  ReferPointer This;
  ReferPointer Ref;
  ReferPointer Last;
  int Compare;

  if( Loc ) {
    This = ( ReferPointer )AssertSpace( &Space, sizeof( ReferRecord ));
    ClaimSpace( &Space, sizeof( ReferRecord ));
    This->Loc = *Loc;
    This->Next = NULL;
    for( Last = NULL, Ref = *Head; Ref; ) {
      Compare = strcmp( Loc->Name, Ref->Loc.Name );
      if( !Compare ) {
        if( Loc->Line < Ref->Loc.Line ) {
          This->Next = Ref;
          Ref = NULL;
        } else {
          Last = Ref;
          Ref = Ref->Next;
        }
      } else 
        if( Compare < 0 ) {
          This->Next = Ref;
          Ref = NULL;
        } else {
          Last = Ref;
          Ref = Ref->Next;
        }
    }
    if( !Last ) 
      *Head = This;
    else  
      if(( Loc->Line != Last->Loc.Line ) || ( Loc->Name != Last->Loc.Name )) 
        Last->Next = This;
  }
}

#if ANSI
  static PROC char *StripSymbol( char *Result, char *Symbol )
#else
  static PROC char *StripSymbol( Result, Symbol )
  char *Result;
  char *Symbol;
#endif
{
  if( *Symbol == SymbolChar ) 
    Symbol++;
  strncpy( Result, Symbol, SymbolLength );
  Result[ SymbolLength ] = NullChar;
  Symbol = Result + strlen( Result ) - 1;
  if( *Symbol == LabelChar )
    *Symbol = NullChar;
  return( Result );
}

#if ANSI
  static PROC char DefineSymbol( char *RawSymbol, long int Value,
    SymbolPointer *Head, LocPointer Loc )
#else
  static PROC char DefineSymbol( RawSymbol, Value, Head, Loc )
  char *RawSymbol;
  long int Value;
  SymbolPointer *Head;
  LocPointer Loc;
#endif
{
  SymbolString Symbol;
  SymbolPointer Symb;
  SymbolPointer Last;
  int Compare;

  StripSymbol( Symbol, RawSymbol );
  Compare = 0;
  Last = NULL;
  Symb = *Head;
  while( Symb ) {
    Compare = strcmp( Symbol, Symb->Symbol );
    if( !Compare ) {
      Symb->Value = Value;
      Symb->Expr = NULL;
      if( Loc ) 
        Symb->Loc = *Loc;
      else
        Symb->Loc = NullLocation; 
      if( Symb->Loc.Xref )
        DefineReference( &( Symb->Loc ), &( Symb->Refer ));
      CurrentSymbol = Symb;
      return( False );
    }
    Last = Symb;
    if( Compare <= 0 )
      Symb = Symb->Left;
    else
      Symb = Symb->Right;
  }
  Symb = ( SymbolPointer )AssertSpace( &Space, sizeof( SymbolRecord ));
  ClaimSpace( &Space, sizeof( SymbolRecord ));
  Symb->Symbol = ( char * )AssertSpace( &Space, sizeof( SymbolString ));
  strcpy( Symb->Symbol, Symbol );
  ClaimSpace( &Space, ( unsigned int )( strlen( Symb->Symbol ) + 1 ));
  Symb->Value = Value;
  Symb->Expr = NULL;
  if( Loc )
    Symb->Loc = *Loc;
  else
    Symb->Loc = NullLocation; 
  Symb->Refer = NULL;
  if( *UsedOnCond )
    DefineReference( &NullLocation, &( Symb->Refer ));
  Symb->Left = NULL;
  Symb->Right = NULL;
  if( !Last )
    *Head = Symb;
  else 
    if( Compare <= 0 )
      Last->Left = Symb;
    else
      Last->Right = Symb;
  CurrentSymbol = Symb;
  return( True );
}

#if ANSI
  static PROC char ResolveOption( char *Option, long int *YesNoPoint,
    long int *OnOffPoint )
#else
  static PROC char ResolveOption( Option, YesNoPoint, OnOffPoint )
  char *Option;
  long int *YesNoPoint;
  long int *OnOffPoint;
#endif
{
  char Result;

  Result = True;
  if( !strcmp( Option, OptionYesName )) {
    *YesNoPoint = Defined;
    Result = False;
  }
  if( !strcmp( Option, OptionNoName )) {
    *YesNoPoint = UnDefined;
    Result = False;
  }
  if( !strcmp( Option, OptionOnName )) {
    *OnOffPoint = Defined;
    Result = False;
  }
  if( !strcmp( Option, OptionOffName )) {
    *OnOffPoint = UnDefined;
    Result = False;
  }
  return( Result );
}

#if ANSI
  static PROC char ResolveSymbol( char *RawSymbol, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveSymbol( RawSymbol, Value, Head, Loc )
  char *RawSymbol;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  SymbolString Symbol;
  int Compare;
  char Result;

  Result = False;
  *Value = 0L;
  StripSymbol( Symbol, RawSymbol );
  while( Head ) {
    Compare = strcmp( Symbol, Head->Symbol );
    if( !Compare ) {
      *Value = Head->Value;
      if( Loc )
        if( Loc->Xref )
          DefineReference( Loc, &( Head->Refer ));
      if( !( Head->Expr ))
        Result = True;
      Head = NULL;
    } else {
      if( Compare <= 0 )
        Head = Head->Left;
      else
        Head = Head->Right;
    }
  }
  return( Result );
}

#if ANSI
  static PROC char ResolveNumber( char *Symbol, int Base, long int *Value )
#else
  static PROC char ResolveNumber( Symbol, Base, Value )
  char *Symbol;
  int Base;
  long int *Value;
#endif
{
  char *Digit;
  char Result;
  unsigned int Count;
  unsigned int Length;

  Result = True;
  if( !( *Symbol ))
    Result = False;
  *Value = 0L;
  Length = strlen( Symbol );
  for( Count = 1; Count <= Length; Count++ ) {
    Digit = strchr( DigitText, Symbol[ Count - 1 ]);
    if( !Digit ) {
      Result = False;
      Count = Length;
    } else {
      if(( Digit - DigitText ) >= MaxBaseLength ) 
        Digit = DigitText;
      if(( Digit - DigitText ) > Base ) {
        Result = False;
        Count = Length;
      } else
        *Value = Base * ( *Value ) + ( Digit - DigitText );
    }
  }
  return( Result );
}

#if ANSI
  static PROC char ResolveValue( char *Symbol, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveValue( Symbol, Value, Head, Loc )
  char *Symbol;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  char Result;

  Result = False;
  *Value = 0L;
  if(( Symbol[ 0 ] == AddressChar ) && !( Symbol[ 1 ]) && Loc ) {
    *Value = Loc->Addr;
    return( True );
  }
  switch( Symbol[ 0 ]) {
    case HexChar:
      Result = ResolveNumber( Symbol + 1, 16, Value );
      break;
    case BinChar:
      Result = ResolveNumber( Symbol + 1, 2, Value );
      break;
    case OctChar:
      Result = ResolveNumber( Symbol + 1, 8, Value );
      break;
    case AscChar:
      if( Symbol[ 1 ]) {
        *Value = Symbol[ 1 ];
        if( !( Symbol[ 2 ]))
        Result = True;
      }
      break;
    default:
      if( isdigit( Symbol[ 0 ]))
        Result = ResolveNumber( Symbol, 10, Value );
      else
        Result = ResolveSymbol( Symbol, Value, Head, Loc );
      break;
  }
  return( Result );
}

#if ANSI
  static PROC char *SplitParam( char *Result, char **Param )
#else
  static PROC char *SplitParam( Result, Param )
  char *Result;
  char **Param;
#endif
{
  char *Start;
  char *Count;
  char *Store;
  char Reading;

  Store = Result;
  *Store = NullChar;
  Reading = True;
  for( Start = NULL, Count = *Param; *Count; Count++ ) 
    if( isgraph( *Count )) {
      Start = Count;
      Count = NullString;
    }
  if( Start ) {
    Count = Start;
    if( Operators[ *Count ]) {
      *( Store++ ) = *Count;
      *( Store++ ) = NullChar; 
      *Param = Count + 1;
      Reading = False;
    } else 
      for( ; *Count; Count++ ) 
        if(( !isgraph( *Count )) || Operators[ *Count ]) {
          *Param = Count;
          Count = NullString;
          Reading = False;
        } else 
          *( Store++ ) = *Count;
  }
  if( Reading )
    *Param = NullString;
  *( Store++ ) = NullChar;
  return( Result );
}

#if ANSI
  static PROC char ResolveOperator( char *Symbol, char *Token, char *Legal )
#else
  static PROC char ResolveOperator( Symbol, Token, Legal )
  char *Symbol;
  char *Token;
  char *Legal;
#endif
{
  *Token = *Symbol;
  if(( *Token ) && strchr( Legal, *Token ))
    return( True ); 
  else
    return( False ); 
}

#if ANSI
  static PROC char ResolveSingle( char Token, long int *Value )
#else
  static PROC char ResolveSingle( Token, Value )
  char Token;
  long int *Value;
#endif
{
  char Result;

  Result = True;
  switch( Token ) {
    case PlusChar:
      /* Nothing to be done */
      break;
    case MinusChar:
      *Value = -( *Value );
      break;
    case NotChar:
      *Value = ~( *Value );
      break;
    default:
      Result = False;
      break;
  }
  return( Result );
}

#if ANSI
  static PROC char ResolveDouble( char Token, long int *Value,
    long int Operand )
#else
  static PROC char ResolveDouble( Token, Value, Operand )
  char Token;
  long int *Value;
  long int Operand;
#endif
{
  long int Count;
  long int This;
  char Result;

  Result = True;
  switch( Token ) {
    case PowerChar:
      This = 1;
      for( Count = 1; Count <= Operand; Count++ )
        This *= *Value;
      *Value = This;
      break;
    case ShlChar:
      *Value <<= Operand;
      break;
    case ShrChar:
      *Value >>= Operand;
      break;
    case MulChar:
      *Value *= Operand;
      break;
    case FracChar:
    case DivChar:
      *Value /= Operand;
      break;
    case PlusChar:
      *Value += Operand;
      break;
    case MinusChar:
      *Value -= Operand;
      break;
    case AndChar:
      *Value &= Operand;
      break;
    case OrChar:
      *Value |= Operand;
      break;
    case XorChar:
      *Value ^= Operand;
      break;
    case ModChar:
      *Value %= Operand;
      break;
    default:
      Result = False;
      break;
  }
  return( Result );
}

#if ANSI
  static PROC char ResolveParent( char *Symbol, char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveParent( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  char Token;
  char Status;

  if( ResolveOperator( Symbol, &Token, StartOperator )) {
    SplitParam( Symbol, Param );
    Status = ResolveDivMod( Symbol, Param, Value, Head, Loc );
    if( !ResolveOperator( Symbol, &Token, StopOperator ))
      Status = False;
    SplitParam( Symbol, Param );
  } else {
    Status = ResolveValue( Symbol, Value, Head, Loc );
    SplitParam( Symbol, Param );
  }
  return( Status );
}

#if ANSI
  static PROC char ResolveUnary( char *Symbol, char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveUnary( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  char Token;
  char Status;

  if( ResolveOperator( Symbol, &Token, UnaryOperator )) {
    SplitParam( Symbol, Param );
    Status = ResolveParent( Symbol, Param, Value, Head, Loc );
    Status = Status & ResolveSingle( Token, Value );
  } else
    Status = ResolveParent( Symbol, Param, Value, Head, Loc );
  return( Status );
}

#if ANSI
  static PROC char ResolvePower( char *Symbol, char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolvePower( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  long int Operand;
  char Token;
  char Status;

  Operand = 0L;
  Status = ResolveUnary( Symbol, Param, Value, Head, Loc );
  while( ResolveOperator( Symbol, &Token, PowerOperator )) {
    SplitParam( Symbol, Param );
    Status = Status & ResolveUnary( Symbol, Param, &Operand, Head, Loc );
    Status = Status & ResolveDouble( Token, Value, Operand );
  }
  return( Status );
}

#if ANSI
  static PROC char ResolveMulDiv( char *Symbol, char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveMulDiv( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  long int Operand;
  char Token;
  char Status;

  Operand = 0L;
  Status = ResolvePower( Symbol, Param, Value, Head, Loc );
  while( ResolveOperator( Symbol, &Token, MulDivOperator )) {
    SplitParam( Symbol, Param );
    Status = Status & ResolvePower( Symbol, Param, &Operand, Head, Loc );
    Status = Status & ResolveDouble( Token, Value, Operand );
  }
  return( Status );
}

#if ANSI
  static PROC char ResolvePlusMinus( char *Symbol, char **Param,
    long int *Value, SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolvePlusMinus( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  long int Operand;
  char Token;
  char Status;

  Operand = 0L;
  Status = ResolveMulDiv( Symbol, Param, Value, Head, Loc );
  while( ResolveOperator( Symbol, &Token, PlusMinusOperator )) {
    SplitParam( Symbol, Param );
    Status = Status & ResolveMulDiv( Symbol, Param, &Operand, Head, Loc );
    Status = Status & ResolveDouble( Token, Value, Operand );
  }
  return( Status );
}

#if ANSI
  static PROC char ResolveBitWise( char *Symbol, char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveBitWise( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  long int Operand;
  char Token;
  char Status;

  Operand = 0L;
  Status = ResolvePlusMinus( Symbol, Param, Value, Head, Loc );
  while( ResolveOperator( Symbol, &Token, BitWiseOperator )) {
    SplitParam( Symbol, Param );
    Status = Status & ResolvePlusMinus( Symbol, Param, &Operand, Head, Loc );
    Status = Status & ResolveDouble( Token, Value, Operand );
  }
  return( Status );
}

#if ANSI
  static PROC char ResolveDivMod( char *Symbol, char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveDivMod( Symbol, Param, Value, Head, Loc )
  char *Symbol;
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  long int Operand;
  char Token;
  char Status;

  Operand = 0L;
  Status = ResolveBitWise( Symbol, Param, Value, Head, Loc );
  while( ResolveOperator( Symbol, &Token, DivModOperator )) {
    SplitParam( Symbol, Param );
    Status = Status & ResolveBitWise( Symbol, Param, &Operand, Head, Loc );
    Status = Status & ResolveDouble( Token, Value, Operand );
  }
  return( Status );
}

#if ANSI
  static PROC char ResolveExpression( char **Param, long int *Value,
    SymbolPointer Head, LocPointer Loc )
#else
  static PROC char ResolveExpression( Param, Value, Head, Loc )
  char **Param;
  long int *Value;
  SymbolPointer Head;
  LocPointer Loc;
#endif
{
  SymbolString Symbol;
  char Result;

  Result = False;
  *Value = 0L;
  SplitParam( Symbol, Param );
  if( *Symbol )
    Result = ResolveDivMod( Symbol, Param, Value, Head, Loc );
  if( *Symbol )
    return( False );
  else
    return( Result );
}

#if ANSI
  static PROC void MissingEquations( SymbolPointer Head )
#else
  static PROC void MissingEquations( Head )
  SymbolPointer Head;
#endif
{
  if( Head ) {
    MissingEquations( Head->Left );
    if( Head->Expr ) {
      SymbPoint = Head;
      RunError( False, UndefinedWarning );
      SymbPoint = NULL;
    }
    MissingEquations( Head->Right ); 
  }
}

#if ANSI
  static PROC unsigned int ResolveTraversal( SymbolPointer Symb,
    SymbolPointer Head )
#else
  static PROC unsigned int ResolveTraversal( Symb, Head )
  SymbolPointer Symb;
  SymbolPointer Head;
#endif
{
  char *Expression;
  long int Value;
  unsigned int Count;

  Count = 0;
  if( Symb ) {
    if( Symb->Expr ) {
      Expression = Symb->Expr;
      if( ResolveExpression( &Expression, &Value, Head, &( Symb->Loc ))) {
        Symb->Value = Value;
        Symb->Expr = NULL; 
      } else
        Count++;
    }
    Count += ResolveTraversal( Symb->Left, Head );
    Count += ResolveTraversal( Symb->Right, Head ); 
  }
  return( Count );
}

#if ANSI
  static PROC void ResolveEquations( SymbolPointer Head )
#else
  static PROC void ResolveEquations( Head )
  SymbolPointer Head;
#endif
{
  unsigned int Count;
  unsigned int Remains;

  Count = 0;
  do {
    Remains = Count;
    Count = ResolveTraversal( Head, Head );
  } while( Count && ( Count != Remains ));
  if( Count ) 
    MissingEquations( Head );
}

#if ANSI
  static PROC void StoreSymbolList( SymbolString SymbolText[],
    unsigned int Min, unsigned int Max, SymbolPointer *DirHead )
#else
  static PROC void StoreSymbolList( SymbolText, Min, Max, DirHead )
  SymbolString SymbolText[];
  unsigned int Min;
  unsigned int Max;
  SymbolPointer *DirHead;
#endif
{
  unsigned int This;

  This = Min + (( Max - Min ) >> 1 );
  if( !DefineSymbol( SymbolText[ This ], ( long int )This, DirHead,
    &NullLocation ))
      RunError( False, CopySymbolWarning );
  if( This != Min )
    StoreSymbolList( SymbolText, Min, This - 1, DirHead );
  if( This != Max )
    StoreSymbolList( SymbolText, This + 1, Max, DirHead );
}

#if ANSI
  static PROC char *SplitLine( char *Result, char **Line, char AbortFlag )
#else
  static PROC char *SplitLine( Result, Line, AbortFlag )
  char *Result;
  char **Line;
  char AbortFlag;
#endif
{
  char *Start;
  char *Count;
  char *Store;
  int Level;
  char TextFlag;
  char Reading;

  Store = Result;
  *Store = NullChar;
  Level = 0;
  TextFlag = False;
  Reading = True;
  for( Start = NULL, Count = *Line; *Count; Count++ ) 
    if( *Count == RemarkChar )
      Count = NullString;
    else 
      if( isgraph( *Count ) && ( *Count != SeparatorChar )) {
        Start = Count;
        Count = NullString;
      }
  if( Start ) 
    for( Count = Start; *Count && ( Count < Start + ParamLength ); Count++ ) 
      if( *Count == TextChar ) {
        if( Count != Start )
          if( *( Count - 1 ) == TextChar ) 
            *( Store++ ) = TextChar;
        TextFlag = ( char )~TextFlag;
      } else
        if( TextFlag ) 
          *( Store++ ) = *Count;
        else
          switch( *Count ) {
            case SeparatorChar:
              if( !Level ) {
                *Line = Count;
                Reading = False;
                Count = NullString;
              } else 
                *( Store++ ) = *Count;
              break;
            case RemarkChar:
              *Line = NullString;
              Reading = False;
              Count = NullString;
              break;
            case StartChar:
              Level++;
              *( Store++ ) = *Count;
              break;
            case StopChar:
              Level--;
              *( Store++ ) = *Count;
              break;
            default:
              if( isgraph( *Count )) 
                *( Store++ ) = ( char )toupper(( int )( *Count ));
              else
                if( AbortFlag && ( !Level )) {
                  *Line = Count;
                  Reading = False;
                  Count = NullString;
                } else
                  if( Store != Result )
                   if( *( Store - 1 ) != SpaceChar )
                     *( Store++ ) = SpaceChar; 
              break;
          }
  if( Reading )
    *Line = NullString;
  if( Store != Result )
    *( Store-- ) = NullChar;
  while( *Store == SpaceChar ) 
    if( Store > Result )
      *( Store-- ) = NullChar;
    else
      *Store = NullChar;
  return( Result );
}

#if ANSI
  static PROC void AlignWordBounds( void )
#else
  static PROC void AlignWordBounds()
#endif
{
  if( *AlignOnCond )
    Location.Addr = (( Location.Addr + 1 ) >> 1 ) << 1;
  if( Location.Addr > FinalAddress )
    FinalAddress = Location.Addr;
  if(( Location.Addr < StartAddress ) || ( Location.Addr > StopAddress ))
    RunError( True, BoundsError );
}

#if ANSI
  static PROC char ParamCheck( unsigned int Count, unsigned int Min,
    unsigned int Max )
#else
  static PROC char ParamCheck( Count, Min, Max )
  unsigned int Count;
  unsigned int Min;
  unsigned int Max;
#endif
{
  if(( Count < Min ) || ( Count > Max ))
    RunError( False, ParamCountWarning );
  if( Count >= Min )
    return( True );
  else
    return( False );
}

#if ANSI
  static PROC char RangeCheck( long int Value, long int Min, long int Max,
    char *Message )
#else
  static PROC char RangeCheck( Value, Min, Max, Message )
  long int Value;
  long int Min;
  long int Max;
  char *Message;
#endif
{
  if(( Value < Min ) || ( Value > Max )) {
    RunError( False, Message );
    return( True );
  } else
    return( False );
}

#if ANSI
  static PROC char DecodeOption( char *Option )
#else
  static PROC char DecodeOption( Option )
  char *Option;
#endif
{
  char Result;

  Result = True;
  if( !strcmp( Option, Chip8Name )) {
    *Chip8Cond = Defined;
    *Chip48Cond = Defined;
    *Super10Cond = UnDefined;
    *Super11Cond = UnDefined;
    *HpHeadCond = UnDefined;
    *HpAscCond = UnDefined;
    Result = False;
  }
  if( !strcmp( Option, Chip48Name )) {
    *Chip8Cond = UnDefined;
    *Chip48Cond = Defined;
    *Super10Cond = UnDefined;
    *Super11Cond = UnDefined;
    *HpHeadCond = Defined;
    Result = False;
  }
  if( !strcmp( Option, Super10Name )) {
    *Chip8Cond = UnDefined;
    *Chip48Cond = UnDefined;
    *Super10Cond = Defined;
    *Super11Cond = UnDefined;
    *HpHeadCond = Defined;
    Result = False;
  }
  if( !strcmp( Option, Super11Name )) {
    *Chip8Cond = UnDefined;
    *Chip48Cond = UnDefined;
    *Super10Cond = Defined;
    *Super11Cond = Defined;
    *HpHeadCond = Defined;
    Result = False;
  }
  if( !strcmp( Option, HpBinName )) {
    *HpHeadCond = Defined;
    *HpAscCond = UnDefined;
    Result = False;
  }
  if( !strcmp( Option, HpAscName )) {
    *HpHeadCond = Defined;
    *HpAscCond = Defined;
    Result = False;
  }
  if( !strcmp( Option, BinaryName )) {
    *HpHeadCond = UnDefined;
    *HpAscCond = UnDefined;
    Result = False;
  }
  if( !strcmp( Option, StringName )) {
    *HpHeadCond = UnDefined;
    *HpAscCond = Defined;
    Result = False;
  }
  return( Result );
}

#if ANSI
  static PROC unsigned int DecodeParameters( char **Line, ParamPointer *Head )
#else
  static PROC unsigned int DecodeParameters( Line, Head )
  char **Line;
  ParamPointer *Head;
#endif
{
  ParamString FoundWord;
  ParamPointer Param;
  ParamPointer Last;
  unsigned int Count;

  *Head = NULL;
  Count = 0;
  for( Last = NULL; **Line; )
    if( *SplitLine( FoundWord, Line, False )) {
      Param = ( ParamPointer )AssertSpace( &Space, sizeof( ParamRecord ));
      ClaimSpace( &Space, sizeof( ParamRecord ));
      Param->Param = ( char * )AssertSpace( &Space, sizeof( ParamString ));
      strcpy( Param->Param, FoundWord );
      ClaimSpace( &Space, ( unsigned int )( strlen( Param->Param ) + 1 ));
      Param->Next = NULL;
      if( Last )
        Last->Next = Param;
      else
        *Head = Param;
      Last = Param;
      Count++;
    }
  return( Count );
}

#if ANSI
  static PROC void DecodeDirective( unsigned int Token, char **Line,
    InstPointer *Head, SymbolPointer Dir, SymbolPointer *Symb,
    SymbolPointer *Cond )
#else
  static PROC void DecodeDirective( Token, Line, Head, Dir, Symb, Cond )
  unsigned int Token;
  char **Line;
  InstPointer *Head;
  SymbolPointer Dir;
  SymbolPointer *Symb;
  SymbolPointer *Cond;
#endif
{
  ParamPointer Params;
  InstPointer Inst;
  unsigned int Count;

  Count = DecodeParameters( Line, &Params );
  switch( Token ) {
    case ElseToken:
      ParamCheck( Count, 0, 0 );
      if( StackPoint == StackStart ) 
        RunError( False, MissingIfdefWarning );
      else 
        *StackPoint = ( char )~( *StackPoint );
      break;
    case EndifToken:
      ParamCheck( Count, 0, 0 );
      if( StackPoint <= StackStart ) 
        RunError( False, MissingIfdefWarning );
      else 
        StackPoint--;
      break;
    case IfdefToken:
    case IfundToken:
      if( ParamCheck( Count, 1, 1 )) {
        long int Value;

        if( !ResolveSymbol( Params->Param, &Value, *Cond, &NullLocation ))
          Value = 0L;
        if( Token == IfundToken )
          Value = ~Value;
        if( StackPoint >= StackStart + StackLength ) 
          RunError( True, NestedIfdefError );
        else 
          if( Value ) 
            *( ++StackPoint ) = True;
          else
            *( ++StackPoint ) = False;
      }
      break;
    default:
      if( *StackPoint ) {
        switch( Token ) {
          case WrongToken:
            RunError( False, InternalWarning );
            break;
          case AlignToken:
            if( ParamCheck( Count, 1, 1 )) 
              if( ResolveOption( Params->Param, AlignOnCond, AlignOnCond ))
                RunError( False, NoOptionWarning );
            break;
          case DefineToken:
          case UndefToken:
            if( ParamCheck( Count, 1, 1 )) 
              if( Token == DefineToken )
                DefineSymbol( Params->Param, Defined, Cond, &NullLocation );
              else
                DefineSymbol( Params->Param, UnDefined, Cond,
                  &NullLocation );
            break;
          case EqualToken:
          case EquToken:
            if( !CurrentSymbol )
              RunError( False, MissingSymbolWarning );
            else 
              if( ParamCheck( Count, 1, 1 )) {
                char *Expression;

                Expression = ( char * )AssertSpace( &Space,
                  sizeof( ParamString ));
                CurrentSymbol->Expr = strcpy( Expression, Params->Param );
                ClaimSpace( &Space, ( unsigned int )
                  ( strlen( Expression ) + 1 ));
              }
            break;
          case DsToken:
            if( ParamCheck( Count, 1, 1 )) {
              char *FirstParam;
              long int Value;

              FirstParam = Params->Param;
              if( !ResolveExpression( &FirstParam, &Value, *Symb, &Location ))
                RunError( False, UndefinedWarning );
              Location.Addr += Value;
            }
            break;
          case OptionToken:
            if( ParamCheck( Count, 1, 1 )) 
              if( DecodeOption( Params->Param ))
                RunError( False, NoOptionWarning );
            break;
          case OrgToken:
            if( ParamCheck( Count, 1, 1 )) {
              char *FirstParam;
              long int Value;

              FirstParam = Params->Param;
              if( ResolveExpression( &FirstParam, &Value, *Symb, &Location ))
                Location.Addr = Value;
              else
                RunError( False, UndefinedWarning );
            }
            break;
          case IncludeToken:
            if( ParamCheck( Count, 1, 1 )) {
              FILE *StoreFile;
              LocRecord StoreLoc;
              char *IncludeName;

              StoreFile = InFile; 
              StoreLoc = Location;
              for( IncludeName = Params->Param; *IncludeName; IncludeName++ )
                *IncludeName = ( char )tolower(( int )( *IncludeName ));
              IncludeName = ( char * )AssertSpace( &Space,
                sizeof( LineString ));
              strcpy( IncludeName, Params->Param );
              ClaimSpace( &Space, ( unsigned int )
                ( strlen( IncludeName ) + 1 ));
              InFile = fopen( IncludeName, "r" );
              if( !InFile )
                RunError( True, NoSourceError );
              DecodeFile( IncludeName, Head, Dir, Symb, Cond );
              InFile = StoreFile;
              Location = StoreLoc;
              if( fprintf( stderr, "Reading: %s\n\n", Location.Name ) <= 0 )
                RunError( True, FileWriteError );
              if( fprintf( ListFile, "Reading: %s\n\n", Location.Name ) <= 0 )
                RunError( True, FileWriteError );
            }
            break;
          case EndToken:
            ParamCheck( Count, 0, 0 );
            /* The END directive is ignored */
            break;
          case UsedToken:
            if( ParamCheck( Count, 1, ListLength )) 
              if( ResolveOption( Params->Param, UsedYesCond, UsedOnCond )) {
                ParamPointer Point;
                long int Value;
      
                for( Point = Params; Point; Point = Point->Next ) 
                  if( *XrefOnCond ) {
                    if( !ResolveSymbol( Point->Param, &Value, *Symb,
                      &Location ))
                        if( !Value )
                          RunError( False, NoSymbolWarning );
                  } else
                    if( !ResolveSymbol( Point->Param, &Value, *Symb,
                      &UsedLocation ))
                        if( !Value )
                          RunError( False, NoSymbolWarning );
              } else
                if( Count > 1 )
                  RunError( False, ParamCountWarning );
            break;
          case XrefToken:
            if( ParamCheck( Count, 1, 1 )) 
              if( ResolveOption( Params->Param, XrefYesCond, XrefOnCond ))
                RunError( False, NoOptionWarning );
            break;
          default:
            Inst = ( InstPointer )AssertSpace( &Space, sizeof( InstRecord ));
            ClaimSpace( &Space, sizeof( InstRecord ));
            Inst->Loc = Location;
            Inst->Token = Token;
            Inst->Count = Count;
            Inst->Params = Params;
            Inst->Next = NULL;
            if( *Head ) 
              ( *Head )->Next = Inst;
            else
              Instructions = Inst;
            *Head = Inst;
            switch( Token ) {
              case DbToken:
                if( ParamCheck( Count, 1, ListLength ))
                  Location.Addr += Count;
                break;
              case DwToken:
                if( ParamCheck( Count, 1, ListLength ))
                  Location.Addr += Count * 2;
                break;
              case DaToken:
                if( ParamCheck( Count, 1, 1 ))
                  Location.Addr += strlen( Params->Param );
                break;
              default:
                Location.Addr += 2;
                break;
            }
          break;
        }
      }
      break;
  }
}

#if ANSI
  static PROC void DecodeLine( char **Line, InstPointer *Head,
    SymbolPointer Dir, SymbolPointer *Symb, SymbolPointer *Cond )
#else
  static PROC void DecodeLine( Line, Head, Dir, Symb, Cond )
  char **Line;
  InstPointer *Head;
  SymbolPointer Dir;
  SymbolPointer *Symb;
  SymbolPointer *Cond;
#endif
{
  ParamString Split;
  long int Value;

  CurrentSymbol = NULL;
  while( **Line ) 
    if( *SplitLine( Split, Line, True )) 
      if( !ResolveSymbol( Split, &Value, Dir, &NullLocation )) {
        if( *StackPoint )
          if( CurrentSymbol ) {
            RunError( False, DualSymbolWarning );
            *Line = NullString;
          } else 
            if( !DefineSymbol( Split, Location.Addr, Symb, &Location ))
              RunError( False, CopySymbolWarning );
      } else {
        DecodeDirective(( unsigned int )Value, Line, Head, Dir, Symb, Cond );
        AlignWordBounds();
        *Line = NullString;
      }
  CurrentSymbol = NULL;
}

#if ANSI
  static PROC void DecodeFile( char *FileName, InstPointer *Head,
    SymbolPointer Dir, SymbolPointer *Symb, SymbolPointer *Cond )
#else
  static PROC void DecodeFile( FileName, Head, Dir, Symb, Cond )
  char *FileName;
  InstPointer *Head;
  SymbolPointer Dir;
  SymbolPointer *Symb;
  SymbolPointer *Cond;
#endif
{
  char *StoreStack;
  char *Line;

  StoreStack = StackPoint;
  if( fprintf( stderr, "Reading: %s\n\n", FileName ) <= 0 )
    RunError( True, FileWriteError );
  if( fprintf( ListFile, "Reading: %s\n\n", FileName ) <= 0 )
    RunError( True, FileWriteError );
  Location.Text = ( char * )AssertSpace( &Space, sizeof( LineString ));
  *( Location.Text ) = NullChar;
  Location.Name = FileName;
  Location.Line = 1;
  while( fgets( Location.Text, LineLength + 1, InFile )) {
    ClaimSpace( &Space, ( unsigned int )( strlen( Location.Text ) + 1 ));
    if( *XrefOnCond )
      Location.Xref = True;
    else
      Location.Xref = False;
    for( Line = Location.Text; *Line; Line++ )
      if( iscntrl( *Line ))
        *Line = SpaceChar;
    Line = Location.Text;
    DecodeLine( &Line, Head, Dir, Symb, Cond );
    Location.Text = ( char * )AssertSpace( &Space, sizeof( LineString ));
    *( Location.Text ) = NullChar;
    Location.Line++;
  }
  strcpy( Location.Text, EndOfFileMessage );
  ClaimSpace( &Space, ( unsigned int )( strlen( Location.Text ) + 1 ));
  if( StoreStack != StackPoint )
    RunError( False, CountDefineWarning );
  if( InFile )
    if( InFile != stdin )
      fclose( InFile );
  InFile = NULL;
  Location = NullLocation;
}

#if ANSI
  static PROC void EncodeNoneToken( InstPointer Inst, long int OpCode,
    long int *Ver10, long int *Ver11 )
#else
  static PROC void EncodeNoneToken( Inst, OpCode, Ver10, Ver11 )
  InstPointer Inst;
  long int OpCode;
  long int *Ver10;
  long int *Ver11;
#endif
{
  ParamCheck( Inst->Count, 0, 0 );
  /* Generate instruction anyway */
  if( !( *Ver11 ))
    RunError( False, NeedsSuper11Warning );
  if( !( *Ver10 ))
    RunError( False, NeedsSuper10Warning );
  Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
    (( OpCode & 0xff00L ) >> 8 );
  Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
    ( OpCode & 0xffL );
  ListInstruction( Inst->Loc.Addr, 2, Inst );
}

#if ANSI
  static PROC void EncodeValToken( InstPointer Inst, long int OpCode,
    long int Limit, SymbolPointer Symb, long int *Ver8, long int *Ver11 )
#else
  static PROC void EncodeValToken( Inst, OpCode, Limit, Symb, Ver8, Ver11 )
  InstPointer Inst;
  long int OpCode;
  long int Limit;
  SymbolPointer Symb;
  long int *Ver8; 
  long int *Ver11;
#endif
{
  long int Value;

  Value = 0L;
  if( ParamCheck( Inst->Count, 1, 1 )) {
    char *FirstParam;

    FirstParam = Inst->Params->Param;
    if( !ResolveExpression( &FirstParam, &Value, Symb, &( Inst->Loc )))
      RunError( False, UndefinedWarning );
  }
  if( RangeCheck( Value, 0L, Limit, RangeWarning ))
    Value &= Limit;
  if( !( *Ver11 ))
    RunError( False, NeedsSuper11Warning );
  if( !( *Ver8 ))
    RunError( False, NeedsChip8Warning );
  Value |= OpCode;
  Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
    (( Value & 0xff00L ) >> 8 );
  Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
    ( Value & 0xffL );
  ListInstruction( Inst->Loc.Addr, 2, Inst );
}

#if ANSI
  static PROC void EncodeRegToken( InstPointer Inst, long int OpCode,
    SymbolPointer Reg )
#else
  static PROC void EncodeRegToken( Inst, OpCode, Reg )
  InstPointer Inst;
  long int OpCode;
  SymbolPointer Reg;
#endif
{
  long int RegX;

  RegX = ( long int )V0Reg;
  if( ParamCheck( Inst->Count, 1, 1 )) {
    if( ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation ))
      RegX -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  }
  if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
    RegX = 0L;
  RegX = OpCode | ( RegX << 8 );
  Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
    (( RegX & 0xff00L ) >> 8 );
  Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
    ( RegX & 0xffL );
  ListInstruction( Inst->Loc.Addr, 2, Inst );
}

#if ANSI
  static PROC void EncodeRegValToken( InstPointer Inst, long int OpCode,
    SymbolPointer Reg, SymbolPointer Symb )
#else
  static PROC void EncodeRegValToken( Inst, OpCode, Reg, Symb )
  InstPointer Inst;
  long int OpCode;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  long int RegX;
  long int Value;

  RegX = ( long int )V0Reg;
  Value = 0L;
  if( Inst->Count >= 1 ) 
    if( ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation ))
      RegX -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
    RegX = 0L;
  if( ParamCheck( Inst->Count, 2, 2 )) {
    char *SecondParam;

    SecondParam = Inst->Params->Next->Param;
    if( !ResolveExpression( &SecondParam, &Value, Symb, &( Inst->Loc )))
      RunError( False, UndefinedWarning );
  }
  if( RangeCheck( Value, 0L, ByteMask, RangeWarning ))
    Value &= ByteMask;
  Value |= OpCode | ( RegX << 8 );
  Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
    (( Value & 0xff00L ) >> 8 );
  Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
    ( Value & 0xffL );
  ListInstruction( Inst->Loc.Addr, 2, Inst );
}

#if ANSI
  static PROC void EncodeRegRegToken( InstPointer Inst, long int OpCode,
    unsigned int Min, SymbolPointer Reg )
#else
  static PROC void EncodeRegRegToken( Inst, OpCode, Min, Reg )
  InstPointer Inst;
  long int OpCode;
  unsigned int Min;
  SymbolPointer Reg;
#endif
{
  long int RegX;
  long int RegY;

  RegX = ( long int )V0Reg;
  RegY = 0L;
  if( ParamCheck( Inst->Count, Min, 2 )) 
    if( ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation ))
      RegX -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
    RegX = 0L;
  if( Inst->Count >= 2 ) 
    if( ResolveSymbol( Inst->Params->Next->Param, &RegY, Reg, &NullLocation ))
      RegY -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  if( RangeCheck( RegY, 0L, NibbleMask, BadRegisterWarning ))
    RegY = 0L;
  RegX = OpCode | ( RegX << 8 ) | ( RegY << 4 );
  Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
    (( RegX & 0xff00L ) >> 8 );
  Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
    ( RegX & 0xffL );
  ListInstruction( Inst->Loc.Addr, 2, Inst );
}

#if ANSI
  static PROC void EncodeRegRegOrValToken( InstPointer Inst, long int OpCode1,
    long int OpCode2, SymbolPointer Reg, SymbolPointer Symb )
#else
  static PROC void EncodeRegRegOrValToken( Inst, OpCode1, OpCode2, Reg, Symb )
  InstPointer Inst;
  long int OpCode1;
  long int OpCode2;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  long int RegX;
  long int RegY;
  long int Value;

  RegX = ( long int )V0Reg;
  RegY = ( long int )V0Reg;
  Value = 0L;
  if( Inst->Count >= 1 ) 
    if( ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation ))
      RegX -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
    RegX = 0L;
  if( ParamCheck( Inst->Count, 2, 2 ))
    if( ResolveSymbol( Inst->Params->Next->Param, &RegY, Reg,
      &NullLocation )) {
        RegY -= V0Reg;
        if( RangeCheck( RegY, 0L, NibbleMask, BadRegisterWarning ))
          RegY = 0L;
        Value = OpCode1 | ( RegX << 8 ) | ( RegY << 4 );
        Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
          (( Value & 0xff00L ) >> 8 );
        Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
          ( Value & 0xffL );
        ListInstruction( Inst->Loc.Addr, 2, Inst );
      } else {
        char *SecondParam;

        SecondParam = Inst->Params->Next->Param; 
        if( !ResolveExpression( &SecondParam, &Value, Symb, &( Inst->Loc )))
          RunError( False, UndefinedWarning );
        if( RangeCheck( Value, 0L, ByteMask, RangeWarning ))
          Value &= ByteMask;
        Value |= OpCode2 | ( RegX << 8 );
        Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
          (( Value & 0xff00L ) >> 8 );
        Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
          ( Value & 0xffL );
        ListInstruction( Inst->Loc.Addr, 2, Inst );
      }
}

#if ANSI
  static PROC void EncodeDrwToken( InstPointer Inst, long int OpCode,
    SymbolPointer Reg, SymbolPointer Symb )
#else
  static PROC void EncodeDrwToken( Inst, OpCode, Reg, Symb )
  InstPointer Inst;
  long int OpCode;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  long int RegX;
  long int RegY;
  long int Value;

  RegX = ( long int )V0Reg;
  RegY = ( long int )V0Reg;
  Value = 0L;
  if( Inst->Count >= 1 ) 
    if( ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation ))
      RegX -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
    RegX = 0L;
  if( Inst->Count >= 2 ) 
    if( ResolveSymbol( Inst->Params->Next->Param, &RegY, Reg, &NullLocation ))
      RegY -= V0Reg;
    else
      RunError( False, NoRegisterWarning );
  if( RangeCheck( RegY, 0L, NibbleMask, BadRegisterWarning ))
    RegY = 0L;
  if( ParamCheck( Inst->Count, 3, 3 )) {
    char *ThirdParam;

    ThirdParam = Inst->Params->Next->Next->Param;
    if( !ResolveExpression( &ThirdParam, &Value, Symb, &( Inst->Loc )))
      RunError( False, UndefinedWarning );
  }
  if( RangeCheck( Value, 0L, NibbleMask, RangeWarning ))
    Value &= NibbleMask;
  if(( Value == 0L ) && !( *Super10Cond ))
    RunError( False, NeedsSuper10Warning );
  Value |= OpCode | ( RegX << 8 ) | ( RegY << 4 );
  Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
    (( Value & 0xff00L ) >> 8 );
  Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
    ( Value & 0xffL );
  ListInstruction( Inst->Loc.Addr, 2, Inst );
}

#if ANSI
  static PROC void EncodeAddToken( InstPointer Inst, SymbolPointer Reg,
    SymbolPointer Symb )
#else
  static PROC void EncodeAddToken( Inst, Reg, Symb )
  InstPointer Inst;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  long int RegX;
  long int RegY;
  long int Value;

  RegX = ( long int )V0Reg;
  RegY = ( long int )V0Reg;
  Value = 0L;
  if( Inst->Count >= 1 ) 
    if( !ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation )) {
      RegX = ( long int )V0Reg;
      RunError( False, NoRegisterWarning );
    }
  if( RegX == ( long int )IReg ) {
    if( ParamCheck( Inst->Count, 2, 2 )) 
      if( ResolveSymbol( Inst->Params->Next->Param, &RegX, Reg,
        &NullLocation ))
          RegX -= V0Reg;
      else
        RunError( False, NoRegisterWarning );
    if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
      RegX = 0L;
    Value = 0xf01eL | ( RegX << 8 );
    Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
      (( Value & 0xff00L ) >> 8 );
    Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
      ( Value & 0xffL );
    ListInstruction( Inst->Loc.Addr, 2, Inst );
  } else {
    RegX -= V0Reg;
    if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
      RegX = 0L;
    if( ParamCheck( Inst->Count, 2, 2 ))
      if( ResolveSymbol( Inst->Params->Next->Param, &RegY, Reg,
        &NullLocation )) {
          RegY -= V0Reg;
          if( RangeCheck( RegY, 0L, NibbleMask, BadRegisterWarning ))
            RegY = 0L;
          Value = 0x8004L | ( RegX << 8 ) | ( RegY << 4 );
          Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
            (( Value & 0xff00L ) >> 8 );
          Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
            ( Value & 0xffL );
          ListInstruction( Inst->Loc.Addr, 2, Inst );
        } else {
          char *SecondParam;

          SecondParam = Inst->Params->Next->Param;
          if( !ResolveExpression( &SecondParam, &Value, Symb, &( Inst->Loc )))
            RunError( False, UndefinedWarning );
          if( RangeCheck( Value, 0L, ByteMask, RangeWarning ))
            Value &= ByteMask;
          Value |= 0x7000L | ( RegX << 8 );
          Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
            (( Value & 0xff00L ) >> 8 );
          Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
            ( Value & 0xffL );
          ListInstruction( Inst->Loc.Addr, 2, Inst );
        }
  }
}

#if ANSI
  static PROC void EncodeJpToken( InstPointer Inst, SymbolPointer Reg,
    SymbolPointer Symb )
#else
  static PROC void EncodeJpToken( Inst, Reg, Symb )
  InstPointer Inst;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  long int RegX;
  long int Addr;

  RegX = ( long int )V0Reg;
  Addr = ( long int )V0Reg;
  if( ParamCheck( Inst->Count, 1, 2 ))
    if( ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation )) {
      if( RegX != V0Reg )
        RunError( False, BadRegisterWarning );
      if( Inst->Count == 1 )
        RunError( False, ParamCountWarning );
      else {
        char *SecondParam;

        SecondParam = Inst->Params->Next->Param;
        if( ResolveExpression( &SecondParam , &Addr, Symb, &( Inst->Loc )))
          RunError( False, UndefinedWarning );
      }
      if( RangeCheck( Addr, 0L, AddrMask, RangeWarning ))
        Addr &= AddrMask;
      Addr |= 0xb000L;
      Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
        (( Addr & 0xff00L ) >> 8 );
      Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
        ( Addr & 0xffL );
      ListInstruction( Inst->Loc.Addr, 2, Inst );
    } else {
      char *FirstParam;

      FirstParam = Inst->Params->Param;
      if( !ResolveExpression( &FirstParam, &Addr, Symb, &( Inst->Loc )))
        RunError( False, UndefinedWarning );
      if( Inst->Count == 2 )
        RunError( False, ParamCountWarning );
      if( RangeCheck( Addr, 0L, AddrMask, RangeWarning ))
        Addr &= AddrMask;
      Addr |= 0x1000L;
      Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
        (( Addr & 0xff00L ) >> 8 );
      Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
        ( Addr & 0xffL );
      ListInstruction( Inst->Loc.Addr, 2, Inst );
    }
}

#if ANSI
  static PROC void EncodeLdToken( InstPointer Inst, SymbolPointer Reg,
    SymbolPointer Symb )
#else
  static PROC void EncodeLdToken( Inst, Reg, Symb )
  InstPointer Inst;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  long int RegX;
  long int RegY;
  long int Value;
  char RegFlag;

  RegX = ( long int )V0Reg;
  RegY = ( long int )V0Reg;
  Value = 0L;
  RegFlag = True;
  if( Inst->Count >= 1 ) 
    if( !ResolveSymbol( Inst->Params->Param, &RegX, Reg, &NullLocation )) {
      RegX = ( long int )V0Reg;
      RunError( False, NoRegisterWarning );
    }
  switch( RegX ) {
    case BReg:
    case DtReg:
    case FReg:
    case HfReg:
    case LfReg:
    case RReg:
    case StReg:
    case IiReg:
      RegY = RegX;
      RegX = ( long int )V0Reg;
      if( ParamCheck( Inst->Count, 2, 2 )) 
        if( ResolveSymbol( Inst->Params->Next->Param, &RegX, Reg,
          &NullLocation ))
            RegX -= V0Reg;
        else
          RunError( False, NoRegisterWarning );
      if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
        RegX = 0L;
      switch( RegY ) {
        case BReg:
          Value = 0xf033L | ( RegX << 8 );
          break;
        case DtReg:
          Value = 0xf015L | ( RegX << 8 );
          break;
        case FReg:
          if( !( *Chip48Cond ))
            RunError( False, NeedsChip48Warning );
          Value = 0xf029L | ( RegX << 8 );
          break;
        case HfReg:
          if( !( *Super10Cond ))
            RunError( False, NeedsSuper10Warning );
          Value = 0xf030L | ( RegX << 8 );
          break;
        case LfReg:
          if( !( *Super10Cond ))
            RunError( False, NeedsSuper10Warning );
          Value = 0xf029L | ( RegX << 8 );
          break;
        case RReg:
          if( RangeCheck( RegX, 0L, 7L, BadRegisterWarning ))
            RegX = 0L;
          if( !( *Super10Cond ))
            RunError( False, NeedsSuper10Warning );
          Value = 0xf075L | ( RegX << 8 );
          break;
        case StReg:
          Value = 0xf018L | ( RegX << 8 );
          break;
        case IiReg:
          Value = 0xf055L | ( RegX << 8 );
          break;
        default:
          RunError( False, InternalWarning );
          break;
      }
      Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
        (( Value & 0xff00L ) >> 8 );
      Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
        ( Value & 0xffL );
      ListInstruction( Inst->Loc.Addr, 2, Inst );
      break;
    case IReg:
      if( ParamCheck( Inst->Count, 2, 2 )) {
        char *SecondParam;

        SecondParam = Inst->Params->Next->Param;
        if( !ResolveExpression( &SecondParam, &Value, Symb, &( Inst->Loc )))
          RunError( False, UndefinedWarning );
      }
      if( RangeCheck( Value, 0L, AddrMask, RangeWarning ))
        Value &= AddrMask;
      Value |= 0xa000L;
      Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
        (( Value & 0xff00L ) >> 8 );
      Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
        ( Value & 0xffL );
      ListInstruction( Inst->Loc.Addr, 2, Inst );
      break;
    default:
      RegX -= V0Reg;
      if( RangeCheck( RegX, 0L, NibbleMask, BadRegisterWarning ))
        RegX = 0L;
      if( ParamCheck( Inst->Count, 2, 2 ))
        RegFlag = ResolveSymbol( Inst->Params->Next->Param, &RegY, Reg,
          &NullLocation );
      if( RegFlag ) 
        switch( RegY ) {
          case DtReg:
            Value = 0xf007L | ( RegX << 8 );
            Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
              (( Value & 0xff00L ) >> 8 );
            Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
              ( Value & 0xffL );
            ListInstruction( Inst->Loc.Addr, 2, Inst );
            break;
          case KReg:
            Value = 0xf00aL | ( RegX << 8 );
            Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
              (( Value & 0xff00L ) >> 8 );
            Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
              ( Value & 0xffL );
            ListInstruction( Inst->Loc.Addr, 2, Inst );
            break;
          case RReg:
            if( RangeCheck( RegX, 0L, 7L, BadRegisterWarning ))
              RegX = 0L;
            if( !( *Super10Cond ))
              RunError( False, NeedsSuper10Warning );
            Value = 0xf085L | ( RegX << 8 );
            Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
              (( Value & 0xff00L ) >> 8 );
            Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
              ( Value & 0xffL );
            ListInstruction( Inst->Loc.Addr, 2, Inst );
            break;
          case IiReg:
            Value = 0xf065L | ( RegX << 8 );
            Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
              (( Value & 0xff00L ) >> 8 );
            Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
              ( Value & 0xffL );
            ListInstruction( Inst->Loc.Addr, 2, Inst );
            break;
          default:
            RegY -= V0Reg;
            if( RangeCheck( RegY, 0L, NibbleMask, BadRegisterWarning ))
              RegY = 0L;
            Value = 0x8000L | ( RegX << 8 ) | ( RegY << 4 );
            Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
              (( Value & 0xff00L ) >> 8 );
            Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
              ( Value & 0xffL );
            ListInstruction( Inst->Loc.Addr, 2, Inst );
            break;
        }
      else {
        if( Inst->Count >= 2 ) {
          char *SecondParam;

          SecondParam = Inst->Params->Next->Param;
          if( !ResolveExpression( &SecondParam, &Value, Symb, &( Inst->Loc )))
            RunError( False, UndefinedWarning );
        }
        if( RangeCheck( Value, 0L, ByteMask, RangeWarning ))
          Value &= ByteMask;
        Value |= 0x6000L | ( RegX << 8 );
        Memory[ Inst->Loc.Addr - StartAddress ] = ( unsigned char )
          (( Value & 0xff00L ) >> 8 );
        Memory[ Inst->Loc.Addr - StartAddress + 1 ] = ( unsigned char )
          ( Value & 0xffL );
        ListInstruction( Inst->Loc.Addr, 2, Inst );
      }
      break;
  }
}

#if ANSI
  static PROC void EncodeDaToken( InstPointer Inst )
#else
  static PROC void EncodeDaToken( Inst )
  InstPointer Inst;
#endif
{
  char *Param;
  char *This;
  long int Store;

  Param = NullString;
  if( Inst->Count >= 1 )
    Param = Inst->Params->Param;
  Store = Inst->Loc.Addr - StartAddress;
  for( This = Param ; *This; This++ )
    Memory[ Store++ ] = *This;
  ListInstruction( Inst->Loc.Addr, ( unsigned int )strlen( Param ), Inst );
}

#if ANSI
  static PROC void EncodeDbToken( InstPointer Inst, SymbolPointer Symb )
#else
  static PROC void EncodeDbToken( Inst, Symb )
  InstPointer Inst;
  SymbolPointer Symb;
#endif
{
  ParamPointer Param;
  char *LoopParam;
  long int Value;
  unsigned int This;

  This = 0;
  Value = 0L;
  for( Param = Inst->Params; Param; Param = Param->Next ) {
    LoopParam = Param->Param; 
    if( !ResolveExpression( &LoopParam, &Value, Symb, &( Inst->Loc )))
      RunError( False, UndefinedWarning );
    if( RangeCheck( Value, 0L, ByteMask, RangeWarning ))
      Value &= ByteMask;
    Memory[ Inst->Loc.Addr + ( This++ ) - StartAddress ] = ( unsigned char )
      Value;
  }
  ListInstruction( Inst->Loc.Addr, Inst->Count, Inst );
}

#if ANSI
  static PROC void EncodeDwToken( InstPointer Inst, SymbolPointer Symb )
#else
  static PROC void EncodeDwToken( Inst, Symb )
  InstPointer Inst;
  SymbolPointer Symb;
#endif
{
  ParamPointer Param;
  char *LoopParam;
  long int Value;
  unsigned int This;

  This = 0;
  Value = 0L;
  for( Param = Inst->Params; Param; Param = Param->Next ) {
    LoopParam = Param->Param;
    if( !ResolveExpression( &LoopParam, &Value, Symb, &( Inst->Loc )))
      RunError( False, UndefinedWarning );
    if( RangeCheck( Value, 0L, WordMask, RangeWarning ))
      Value &= WordMask;
    Memory[ Inst->Loc.Addr + ( This++ ) - StartAddress ] = ( unsigned char )
      (( Value & 0xff00L ) >> 8 );
    Memory[ Inst->Loc.Addr + ( This++ ) - StartAddress ] = ( unsigned char )
      ( Value & 0xffL );
  }
  ListInstruction( Inst->Loc.Addr, Inst->Count * 2, Inst );
}

#if ANSI
  static PROC void EncodeInstruction( InstPointer Inst, SymbolPointer Reg,
    SymbolPointer Symb )
#else
  static PROC void EncodeInstruction( Inst, Reg, Symb )
  InstPointer Inst;
  SymbolPointer Reg;
  SymbolPointer Symb;
#endif
{
  switch( Inst->Token ) {
    case AddToken:
      EncodeAddToken( Inst, Reg, Symb );
      break;
    case AndToken:
      EncodeRegRegToken( Inst, 0x8002L, 2, Reg );
      break;
    case CallToken:
      EncodeValToken( Inst, 0x2000L, AddrMask, Symb, &DefinedValue,
        &DefinedValue );
      break;
    case ClsToken:
      EncodeNoneToken( Inst, 0x00e0L, &DefinedValue, &DefinedValue );
      break;
    case DaToken:
      EncodeDaToken( Inst );
      break;
    case DbToken:
      EncodeDbToken( Inst, Symb );
      break;
    case DrwToken:
      EncodeDrwToken( Inst, 0xd000L, Reg, Symb );
      break;
    case DwToken:
      EncodeDwToken( Inst, Symb );
      break;
    case ExitToken:
      EncodeNoneToken( Inst, 0x00fdL, Super10Cond, &DefinedValue );
      break;
    case HighToken:
      EncodeNoneToken( Inst, 0x00ffL, Super10Cond, &DefinedValue );
      break;
    case JpToken:
      EncodeJpToken( Inst, Reg, Symb );
      break;
    case LdToken:
      EncodeLdToken( Inst, Reg, Symb );
      break;
    case LowToken:
      EncodeNoneToken( Inst, 0x00feL, Super10Cond, &DefinedValue );
      break;
    case OrToken:
      EncodeRegRegToken( Inst, 0x8001L, 2, Reg );
      break;
    case RetToken:
      EncodeNoneToken( Inst, 0x00eeL, &DefinedValue, &DefinedValue );
      break;
    case RndToken:
      EncodeRegValToken( Inst, 0xc000L, Reg, Symb );
      break;
    case ScdToken:
      EncodeValToken( Inst, 0x00c0L, NibbleMask, Symb, &DefinedValue,
        Super11Cond );
      break;
    case SclToken:
      EncodeNoneToken( Inst, 0x00fcL, &DefinedValue, Super11Cond );
      break;
    case ScrToken:
      EncodeNoneToken( Inst, 0x00fbL, &DefinedValue, Super11Cond );
      break;
    case SeToken:
      EncodeRegRegOrValToken( Inst, 0x5000L, 0x3000L, Reg, Symb );
      break;
    case ShlToken:
      EncodeRegRegToken( Inst, 0x800eL, 1, Reg );
      break;
    case ShrToken:
      EncodeRegRegToken( Inst, 0x8006L, 1, Reg );
      break;
    case SknpToken:
      EncodeRegToken( Inst, 0xe0a1L, Reg );
      break;
    case SkpToken:
      EncodeRegToken( Inst, 0xe09eL, Reg );
      break;
    case SneToken:
      EncodeRegRegOrValToken( Inst, 0x9000L, 0x4000L, Reg, Symb );
      break;
    case SubToken:
      EncodeRegRegToken( Inst, 0x8005L, 2, Reg );
      break;
    case SubnToken:
      EncodeRegRegToken( Inst, 0x8007L, 2, Reg );
      break;
    case SysToken:
      EncodeValToken( Inst, 0x2000L, AddrMask, Symb, Chip8Cond,
        &DefinedValue );
      break;
    case XorToken:
      EncodeRegRegToken( Inst, 0x8003L, 2, Reg );
      break;
    default:
      RunError( False, InternalWarning );
      break;
  }
}

#if ANSI
  static PROC void EncodeMemory( InstPointer Inst, SymbolPointer Reg,
    SymbolPointer Symb, SymbolPointer Cond )
#else
  static PROC void EncodeMemory( Inst, Reg, Symb, Cond )
  InstPointer Inst;
  SymbolPointer Reg;
  SymbolPointer Symb;
  SymbolPointer Cond;
#endif
{
  long int Count;

  if( fprintf( ListFile, "-----   *INSTRUCTIONS*   -----\n\n" ) <= 0 )
    RunError( True, FileWriteError );
  for( Count = StartAddress; Count <= StopAddress; Count++ )
    Memory[ Count - StartAddress ] = 0;
  while( Inst ) {
    InstPoint = Inst;
    EncodeInstruction( Inst, Reg, Symb );
    Inst = Inst->Next;
  }
  InstPoint = NULL;
  if( fprintf( ListFile, "\n-----   *SYMBOLS*   -----\n\n" ) <= 0 )
    RunError( True, FileWriteError );
  ListSymbols( Symb );
  if( fprintf( ListFile, "\n-----   *CONDITIONS*   -----\n\n" ) <= 0 )
    RunError( True, FileWriteError );
  ListDefines( Cond );
  fprintf( ListFile, "\n" );
  ListWarnings();
}

#if ANSI
  static PROC void WriteCheckByte( long int *Check, unsigned char Value )
#else
  static PROC void WriteCheckByte( Check, Value )
  long int *Check;
  unsigned char Value;
#endif
{
  SymbolString MsbText;
  SymbolString LsbText;

  if( *HpHeadCond ) {
    if( fprintf( OutFile, "%s%s", NumberString( LsbText,
      ( long int )( Value & NibbleMask ), 16, 1 ), NumberString( MsbText,
      ( long int )(( Value >> 4 ) & NibbleMask ), 16, 1 )) <= 0 )
        RunError( True, FileWriteError );
  } else
    if( fprintf( OutFile, "%s", NumberString( MsbText, ( long int )Value,
      16, 2 )) <= 0 )
        RunError( True, FileWriteError );
  *Check = ((( *Check ) >> 4 ) ^ ((( Value ^ ( *Check )) & NibbleMask ) *
    CheckMagic ) & WordMask );
  *Check = ((( *Check ) >> 4 ) ^ (((( Value >> 4 ) ^ ( *Check )) &
    NibbleMask ) * CheckMagic ) & WordMask );
}

#if ANSI
  static PROC void WriteMemory( long int Start, long int Stop )
#else
  static PROC void WriteMemory( Start, Stop )
  long int Start;
  long int Stop;
#endif
{
  if( *HpAscCond ) {
    OutFile = freopen( OutFileName, "w+", OutFile );
    if( !OutFile )
      RunError( True, FileAccessError );
  }
  if( *HpHeadCond )
    if( *HpAscCond ) {
      if( fprintf( OutFile, "%s", AscHpHeadText ) <= 0 )
        RunError( True, FileWriteError );
    } else {
      unsigned char HpHeading[ BinHpHeadLength ];
      long int Size;

      Size = (( Stop - Start ) << 1 ) + 5;
      strcpy(( char * )HpHeading, BinHpHeadText );
      HpHeading[ BinHpHeadLength - 5 ] = 0x2c;
      HpHeading[ BinHpHeadLength - 4 ] = 0x2a;
      HpHeading[ BinHpHeadLength - 3 ] = ( unsigned char )( 0 | (( Size &
        0xfL ) << 4 ));
      HpHeading[ BinHpHeadLength - 2 ] = ( unsigned char )(( Size &
        0xff0L ) >> 4 );
      HpHeading[ BinHpHeadLength - 1 ] = ( unsigned char )(( Size &
        0xff000L ) >> 12 );
      if( fwrite(( char * )HpHeading, 1, BinHpHeadLength, OutFile ) !=
        BinHpHeadLength ) 
          RunError( True, FileWriteError );
    }
  if( *HpAscCond ) {
    SymbolString HexText;
    long int Count;
    long int Check;

    Check = 0L;
    if( *HpHeadCond ) {
      long int Size;

      Size = (( Stop - Start ) << 1 ) + 5;
      if( fprintf( OutFile, "\"" ) <= 0 )
        RunError( True, FileWriteError );
      WriteCheckByte( &Check, 0x2c );
      WriteCheckByte( &Check, 0x2a );
      WriteCheckByte( &Check, ( unsigned char )( 0 | (( Size & 0xfL ) << 4 )));
      WriteCheckByte( &Check, ( unsigned char )(( Size & 0xff0L ) >> 4 ));
      WriteCheckByte( &Check, ( unsigned char )(( Size & 0xff000L ) >> 12 ));
    }
    for( Count = Start; Count < Stop; Count++ ) {
      WriteCheckByte( &Check, Memory[ Count - StartAddress ]);
      if( !(( Count - Start + 6 ) % 32 ) && ( *HpHeadCond ))
        if( fprintf( OutFile, "\n" ) <= 0 )
          RunError( True, FileWriteError );
    }
    if( *HpHeadCond ) {
      for( Count = 0; Count < 4; Count++ ) 
        if( fprintf( OutFile, "%s", NumberString( HexText,
          ( Check >> ( 4 * Count )) & NibbleMask, 16, 1 )) <= 0 )
            RunError( True, FileWriteError );
      if( fprintf( OutFile, "\"" ) <= 0 )
        RunError( True, FileWriteError );
    }
  } else {
    unsigned int Size;

    if(( Stop - Start ) > 0L ) {
      Size = ( unsigned int )( Stop - Start );
      if( fwrite(( char * )&Memory[ Start - StartAddress ], 1, Size,
        OutFile ) != Size )
          RunError( True, FileWriteError );
    }
  }
}

#if ANSI
  PROC int main( int argc, char *argv[] )
#else
  PROC int main( argc, argv )
  int argc;
  char *argv[];
#endif
{
  InstPointer Inst; 
  unsigned int Count;
  
  if( fprintf( stderr, "%s\n\n", CopyRight ) <= 0 )
    RunError( True, FileWriteError );
  strcpy( Separator, "---------------------------------------" );
  strcat( Separator, "---------------------------------------" );
  for( Count = 0x0; Count < 0x100; Count++ )
    Operators[ Count ] = False;
  Operators[ StartChar ] = True;
  Operators[ StopChar ] = True;
  Operators[ PlusChar ] = True;
  Operators[ MinusChar ] = True;
  Operators[ NotChar ] = True;
  Operators[ PowerChar ] = True;
  Operators[ ShlChar ] = True;
  Operators[ ShrChar ] = True;
  Operators[ MulChar ] = True;
  Operators[ FracChar ] = True;
  Operators[ AndChar ] = True;
  Operators[ OrChar ] = True;
  Operators[ XorChar ] = True;
  Operators[ DivChar ] = True;
  Operators[ ModChar ] = True;
  if( OpenFiles( argc, argv )) {
    *StackPoint = True;
    StoreSymbolList( TokenText, 0, LastToken - 1, &Directives );
    StoreSymbolList( RegisterText, 0, LastReg - 1, &Registers );
    DefineSymbol( Super10Name, Super10Default, &Conditions, &NullLocation );
    Super10Cond = &( CurrentSymbol->Value );
    DefineSymbol( Chip48Name, Chip48Default, &Conditions, &NullLocation );
    Chip48Cond = &( CurrentSymbol->Value );
    DefineSymbol( UsedYesName, UsedYesDefault, &Conditions, &NullLocation );
    UsedYesCond = &( CurrentSymbol->Value );
    DefineSymbol( AlignOnName, AlignOnDefault, &Conditions, &NullLocation );
    AlignOnCond = &( CurrentSymbol->Value );
    DefineSymbol( HpAscName, HpAscDefault, &Conditions, &NullLocation );
    HpAscCond = &( CurrentSymbol->Value );
    DefineSymbol( Super11Name, Super11Default, &Conditions, &NullLocation );
    Super11Cond = &( CurrentSymbol->Value );
    DefineSymbol( XrefOnName, XrefOnDefault, &Conditions, &NullLocation );
    XrefOnCond = &( CurrentSymbol->Value );
    DefineSymbol( Chip8Name, Chip8Default, &Conditions, &NullLocation );
    Chip8Cond = &( CurrentSymbol->Value );
    DefineSymbol( HpHeadName, HpHeadDefault, &Conditions, &NullLocation );
    HpHeadCond = &( CurrentSymbol->Value );
    DefineSymbol( UsedOnName, UsedOnDefault, &Conditions, &NullLocation );
    UsedOnCond = &( CurrentSymbol->Value );
    DefineSymbol( XrefYesName, XrefYesDefault, &Conditions, &NullLocation );
    XrefYesCond = &( CurrentSymbol->Value );
    CurrentSymbol = NULL;
    Inst = NULL;
    DecodeFile( InFileName, &Inst, Directives, &Symbols, &Conditions );
    if( fprintf( stderr, "Done reading\n\n" ) <= 0 )
      RunError( True, FileWriteError );
    if( fprintf( ListFile, "Done reading\n\n" ) <= 0 )
      RunError( True, FileWriteError );
    ResolveEquations( Symbols );
    EncodeMemory( Instructions, Registers, Symbols, Conditions );
    WriteMemory( StartAddress, FinalAddress );
    if( OutFile )
      fclose( OutFile );
    OutFile = NULL;
    if( ListFile )
      if( ListFile != stdout )
        fclose( ListFile );
    ListFile = NULL;
    ReleaseSpace( &Space );
  }
  return( SuccessExitCode );
}

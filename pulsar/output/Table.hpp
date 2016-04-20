#ifndef _TABLE_H_
#define _TABLE_H_

#include <string>
#include <vector>
namespace bpmodule{
namespace output{


/** \page Printing pretty tables with BPModule
 *    
 *   \todo Go back over and C++11-ify this 
 *  
 *   \tableofcontents
 *
 *   \section sec1 Introduction
 *
 *   Many results gathered in electronic structure theory garner themselves
 *   to being printed in tables, but making a nice looking table can be
 *   a hassle owing to the amount of formatting required.  The series of
 *   classes in this header file are designed to fix that.
 *
 *   \section sec14 Capabilities
 *
 *   As of right now this class should be able to support any rectangular
 *   table and any data that can be boost::lexical_cast to a string.
 *   The number of columns and rows is arbitrary.  You can pick from
 *   three alignments within a cell (left, center, right).  The border of
 *   each cell is customizable, as is the border of the table.  By being
 *   creative within the row class it should be possible to emulate the
 *   Latex multicolumn command and fathomably the multirow command.  Not
 *   sure what other options one wants.
 *
 *   \section sec2 History
 *
 *   It's always good to keep track of what you did and why it didn't work
 *
 *   From the get go the table class has always been designed to be general
 *   in that it could print arbitrary data, in arbitrary layouts.  Arbitrary
 *   data suggests the use of templates; however, this incarnation avoids this
 *   as will be seen.  Anyways, the first try was C++98 compliant and relied
 *   on template recursion to print the table.  It had two serious problems:
 *   1) The table was limited to 10 columns (which isn't much when you are
 *   printing say a 4 index quantity and a couple values relating to it) and
 *   the data had to be copied into the table, sometimes with a transpose.
 *   My second table class used variadic templates (hence only C++11 compliant)
 *   and added the concept of a TableColumn class.  The variadic template
 *   was nice because we were no longer bound to 10 columns, and the
 *   TableColumn class was nice because it avoided the transpose problems in
 *   a creative way.
 *
 *   My next thought was that in the end everything that goes into the
 *   table needs to be printed, this suggests that rather than having to
 *   deal with a templated Table class maybe I can deal with lexical casts
 *   of arbitrary data.  The results are all of type std::string and hence
 *   I don't have to worry about templates. I then wanted to add options for
 *   making Row classes or Column classes; however the logic of what trumps
 *   what got too complicated (if cell 2,3 is specified in both a row and
 *   a column who prints cell 2,3?, how do I know the cell is empty)
 *
 *   The Lexical cast is a great idea, but the two separate classes are too
 *   much.  What if I just emulate Excel?  The base class is broken into
 *   cells.  The table keeps track of the character between columns and
 *   rows.  We then provide fill column and fill row options, as
 *   well as equivalent options for alignment and borders.  This seems
 *   to work well and is the current implementation.
 *
 *   \section sec3 Usage
 *
 *   So how do you use the table class?  Pretty simple.  For concreteness,
 *   assume we have the following data:
 *   \code
 *   double DataPoints[]={1.0, 1.1, 1.2,
 *                        2.0, 2.1, 2.2,
 *                        3.0, 3.1, 3.2};
 *   std::string ColTitles[]={"Col1","Col2","Col3"};
 *   std::string RowTitles[]={"Row1","Row2","Row3"};
 *   \endcode
 *
 *   This data is naturally arranged into a table that is 4 columns
 *   wide and 4 rows long (a row for the column titles and a column
 *   for the row titles).  We declare our table and fill it:
 *   \code
 *   //Makes a 4x4 table
 *   Table OurTable(4,4);
 *
 *   //Add the column titles which go from column 1 to 3
 *   OurTable.AddRow(ColTitles,1,3);

 *   //Add the row titles which go from row 1 to 3
 *   OurTable.AddCol(RowTitles,1,3);
 *
 *   //Add the data
 *   for(i=0;i<3;i++)
 *      OurTable.AddCol(&DataPoints[i],//Get a pointer to our 1st element
 *                      1,3,//puts it in rows 1 to 3
 *                      3);//Stride between adjacent data points
 *
 *   //To print the table to standard out
 *   std::cout<<OurTable;
 *
 *   \endcode
 *
 *   The above code is fine for a quick and dirty table, but often you
 *   want more pizazz.  For that reason you can add vertical and
 *   horizontal delimiters to your table as well:
 *
 *   \code
 *   //If we wanted a row of '*' above and below the table:
 *   //Above row 0
 *   OurTable.SetHBorder(0,'*');
 *   //Above row 4 (i.e. under row 3)
 *   OurTable.SetHBorder(4,'*');
 *
 *   //If we wanted a column of '|' between column 1 and 2:
 *   //To the left of column 1
 *   OurTable.SetVBorder(1,'|');
 *
 *   //Say we wanted column 1 to be left aligned
 *   OurTable.SetAlign(LEFT);
 *   \endcode
 *
 *   For even finer control I allow you to manually tweak any border,
 *   width, or alignment, but be warned I haven't tested the
 *   following well, but they operate under the same principles
 *   as the examples above so I anticipate it working as advertised:
 *   \code
 *
 *   //Make cell (0,1) have a border of "|"
 *   //First add a space so that the columns still line up
 *   SetVBorder(1,' ');
 *   //Now change cell (0,1)'s border
 *   GetVBorder(0,1)='|';
 *
 *
 *   //Right align cell (2,3)
 *   GetCell(2,3).SetAlign(RIGHT);
 *
 *   //Change its width
 *   GetCell(2,3).SetWidth(5);
 *   \endcode
 *
 *   Using the last two code sections one should be able to set
 *   all of the parameters to their liking.  Pay particular attention
 *   to the first example shown in the code above as this is a gothca
 *   if you don't want borders on every cell in the column, but you
 *   still want the cells to line up.
 *
 */

///The possible alignments (add support for justified?)
enum Align{LEFT,CENTER,RIGHT};

///Class that holds the key ingredients of a cell of a table
class Cell{
   private:
      ///How many characters wide the cell is
      size_t Width_;
      ///The actual data
      std::string Data_;
      ///My alignment in the cell
      Align Align_;
   public:
      Cell():Width_(0),Data_(" "),Align_(CENTER){}
      ///Returns the width of the cell
      size_t Width()const{return Width_;}
      ///Adds an arbitrary piece of data to the cell via lexical cast
      template<typename T>
      void AddData(const T& Data){
         Data_=std::to_string(Data);
      }
      void AddData(const std::string& Data);
      void AddData(const double& Data);
      
      ///Sets the width of the cell
      void SetWidth(size_t Width){Width_=Width;}
      ///Sets the alignment of the cell
      void SetAlign(Align Alignment){Align_=Alignment;}
      ///Returns the cell as a string
      std::string PrintOut(size_t Width)const;
};

///A class that holds cells and borders
class Table{
private:
   ///The actual table that will be printed
   std::vector<std::vector<Cell> > DaTable_;
   ///The total number of characters the table will occupy
   size_t Width_;
   ///They type of the borders
   typedef std::vector<char> Border_t;
   
   ///An array of the characters to be used as horizontal borders
   std::vector<Border_t> HBorders_;
   ///An array of the characters to be used as vertical borders
   std::vector<Border_t> VBorders_;
   public:
      ///Makes an NRows by NCols table that is NChars wide
      Table(size_t NRows,size_t NCols,size_t NChars=80);

      ///Returns cell (RowI,ColI)
      Cell GetCell(size_t RowI,size_t ColI)const;
      Cell& GetCell(size_t RowI,size_t ColI);


      ///Returns the border left of cell (RowI,ColI)
      char GetVBorder(size_t RowI,size_t ColI)const;
      char& GetVBorder(size_t RowI,size_t ColI);
      
      ///Returns the border above cell (RowI,ColI)
      char GetHBorder(size_t RowI,size_t ColI)const;
      char& GetHBorder(size_t RowI,size_t ColI);

      ///Fills (RowI,ColI) to (RowEnd-1,ColI) with data from Data
      template<typename T>
      void FillCol(const T& Data,size_t ColI,
            size_t RowI,size_t RowEnd, size_t Offset=1){
         for(size_t J=RowI;J<RowEnd;J++)
         DaTable_[J][ColI].AddData(Data[(J-RowI)*Offset]);
      }
      
      ///Fills (RowI,ColI) to (RowI,ColEnd-1) with data from Data
      template<typename T>
      void FillRow(const T& Data,size_t RowI,
                   size_t ColI,size_t ColEnd,size_t Offset=1){
         for(size_t I=ColI;I<ColEnd;I++)
            DaTable_[RowI][I].AddData(Data[(I-ColI)*Offset]);      }
      
      ///Sets the width of \p ColI to \p width
      void SetWidth(size_t ColI,size_t width);
      ///Sets the alignment of ColI to align
      void SetAlign(size_t ColI,Align align);
      ///Sets border left of \p ColI to \p bord (ColI=NCols for right border)
      void SetVBorder(size_t ColI,char bord);
      ///Sets border above of \p RowI to \p bord (RowI=NRows for bottom border)
      void SetHBorder(size_t RowI,char bord);

      ///Prints the table to an ostream
      void Print(std::ostream & os) const;

};
    
std::ostream& operator<<(std::ostream& os,const Table& tab){
    tab.Print(os);
    return os;
}

}}//End namespaces


#endif 

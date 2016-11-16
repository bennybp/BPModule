#ifndef PULSAR_GUARD_OUTPUT__TABLE_HPP_
#define PULSAR_GUARD_OUTPUT__TABLE_HPP_

#include <string>
#include <vector>
namespace pulsar{

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

}//End namespaces


#endif 

#include <sstream>
#include<iostream>
#include<iomanip>
#include<limits>
#include "pulsar/output/Table.hpp"
#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/math/Cast.hpp"

typedef std::vector<char> Border_t;
namespace pulsar{

        typedef std::vector<Cell> Col_t;
        typedef std::vector<Col_t> Row_t;

        
        Table::Table(size_t NRows,size_t NCols,size_t NChars):
                DaTable_(NRows,Col_t(NCols)),Width_(NChars),
                HBorders_(NRows+1,Border_t(NCols,'\0')),
                VBorders_(NRows,Border_t(NCols+1,'\0')){ }

        //Given a Row, determine the column widths
        std::vector<size_t> GetWidths(Row_t::const_iterator RowI,size_t NCols,
                                      const Border_t& VBorders_,size_t Width_){
            std::vector<size_t> Widths(NCols,0);
            size_t PreSetWidths=0,counter2=0;
            //Remove all preset columns
            for(const auto& ColI : *RowI){
                size_t Temp=ColI.Width();
                if(Temp>0){
                    PreSetWidths+=Temp;
                    NCols--;
                    Widths[++counter2]=Temp;
                }
            }
            //Remove characters for vertical borders
            for(const auto& BorderI: VBorders_)
                if(BorderI!='\0')PreSetWidths++;
            //User has set all column widths manually
            if(NCols==0)return Widths;
            size_t Remainder=(Width_-PreSetWidths)%NCols;
            size_t EffColumn=(Width_-PreSetWidths-Remainder)/NCols;
            for(size_t i=0;i<Widths.size();i++){
                if(Widths[i]==0){
                    Widths[i]=Remainder+EffColumn;
                    Remainder=0;
                }
            }
            return Widths;
        }
        
        void PrintHBorder(size_t NCols,
                                 std::ostream& Result,
                                 std::vector<size_t>& Widths,
                                 const Border_t& VBorders_,
                                 const Border_t& HBorders_){
            bool Print=false;
            for(size_t i=0;i<NCols;i++){
                if(HBorders_[i]=='\0')continue;
                //Print it over the vertical border if it exists
                if(VBorders_[i]!='\0')Result<<HBorders_[i];
                //Print it over the cell
                Result<<std::string(Widths[i],HBorders_[i]);
                Print=true;
            }
            //Print it over the last VBorder if it exists
            if(VBorders_.back()!='\0'&&HBorders_.back()!='\0')
                Result<<HBorders_.back();
            if(Print)Result<<std::endl;
        }
        
        std::string Cell::PrintOut(size_t Width)const{
            std::stringstream Result;
            size_t length=Data_.length();
            if(Width<length)
                throw 
                    PulsarException("The column is not wide enough for this string: ");
                size_t Pad=Width-length;
            if(Align_==CENTER)
                Result<<std::string((Pad+Pad%2)/2,' ')
                <<Data_
                    <<std::string((Pad-Pad%2)/2,' ');
            else if(Align_==LEFT||Align_==RIGHT)
                Result<<std::setfill(' ')<<std::setw(numeric_cast<int>(Width))
                <<(Align_==LEFT?std::left:std::right)<<Data_;
            return Result.str();
        }
        
        void Table::Print(std::ostream& Result)const{
            Row_t::const_iterator RowI=DaTable_.begin(),RowEnd=DaTable_.end();
            std::vector<size_t> Widths;
            for(size_t counter=0;RowI!=RowEnd;++counter){
                size_t NCols=RowI->size();
                Widths=
                        GetWidths(RowI,NCols,VBorders_[counter],Width_);
                PrintHBorder(NCols,Result,Widths,VBorders_[counter],HBorders_[counter]);

                //Print the row
                Col_t::const_iterator ColI=RowI->begin(),ColEnd=RowI->end();
                for(size_t counter1=0;ColI!=ColEnd;counter1++){
                    if(VBorders_[counter][counter1]!='\0')
                        Result<<VBorders_[counter][counter1];
                    Result<<ColI->PrintOut(Widths[counter1]);
                    ++ColI;
                    if(ColI==ColEnd){//Print right VBorder
                        if(VBorders_[counter][counter1+1]!='\0')
                            Result<<VBorders_[counter][counter1+1];
                    }
                }
                Result<<std::endl;
                ++RowI;
                if(RowI==RowEnd)//Print bottom HBorder
                    PrintHBorder(NCols,Result,Widths,VBorders_[counter],HBorders_[counter+1]);
            }
        }
        
        void Cell::AddData(const std::string& Data){
          Data_=Data;
        }
        void Cell::AddData(const double& Data){
          std::stringstream ss;
          ss<<std::setprecision(std::numeric_limits<double>::max_digits10)
             <<std::fixed
             <<std::setw(std::numeric_limits<double>::max_digits10)<<Data;
          Data_=ss.str();
        }
        
        void Table::SetHBorder(size_t RowI,char bord){
            for(size_t i=0;i<DaTable_[0].size();i++)
                HBorders_[RowI][i]=bord;
        }
        void Table::SetVBorder(size_t ColI,char bord){
            for(size_t i=0;i<DaTable_.size();i++)
                VBorders_[i][ColI]=bord;
        }
        void Table::SetAlign(size_t ColI,Align align){
            for(size_t i=0;i<DaTable_.size();i++)
                DaTable_[i][ColI].SetAlign(align);
        }
        void Table::SetWidth(size_t ColI,size_t width){
            for(size_t i=0;i<DaTable_.size();i++)
                DaTable_[i][ColI].SetWidth(width);
        }
        char& Table::GetHBorder(size_t RowI,size_t ColI){
            return HBorders_[RowI][ColI];
        }
        char& Table::GetVBorder(size_t RowI,size_t ColI){
            return VBorders_[RowI][ColI];
        }
        char Table::GetHBorder(size_t RowI,size_t ColI)const{
            return HBorders_[RowI][ColI];
        }
        char Table::GetVBorder(size_t RowI,size_t ColI)const{
            return VBorders_[RowI][ColI];
        }
        Cell Table::GetCell(size_t RowI,size_t ColI)const{
            return DaTable_[RowI][ColI];
        }
        Cell& Table::GetCell(size_t RowI,size_t ColI){
            return DaTable_[RowI][ColI];
        }

}//End namespaces



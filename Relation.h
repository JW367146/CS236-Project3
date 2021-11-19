
#ifndef CS236_PROJECT1_RELATION_H
#define CS236_PROJECT1_RELATION_H


#include "Tuple.h"
#include "Header.h"


class Relation{

private:

    Header header;
    std::string name;
    std::set<Tuple> tuples;




public:
    Relation(){}
    Relation(std::string name){
        this->name = name;
    }
    Relation(std::string name, Header header){
        this->header = header;
        this->name = name;
    }
    Relation(std::string namen, Header header, std::set<Tuple> tuples){
        //For when I want to make a full relation off the bat.
        this->header = header;
        this->name = name;
        this->tuples = tuples;
    }

    void insertTuple(Tuple tuple){
        tuples.insert(tuple);
    }
    std::vector<std::string> getAttributes(){
        return header.getAttributes();
    }
    void setName(std::string name){
        this->name = name;
    }



    Relation select(Relation* input, int attributeIndex, std::string constant ){

        //I'm going to copy the relation I just passed in
        Relation output = Relation(input->name, input->header);


        //Need to iterate over the set of tuples and remove the ones that don't have the value.
        std::set<Tuple>::iterator it;
        //First let's just find which ones will be removed and remove them after.
        std::vector<std::set<Tuple>::iterator> iterators;
        for(it = input->tuples.begin();it != input->tuples.end(); ++it){
            Tuple currentTuple = *it;
           if(currentTuple.valueAtI(attributeIndex) != constant){
               iterators.push_back(it);
           }
        }
        //Now we need to remove from the set of tuples all the tuples we found.
        for(unsigned int i = 0; i < iterators.size();i++){
            input->tuples.erase(iterators.at(i));
        }
        output.tuples = input->tuples;
        return output;
    }

    Relation project(Relation* input, int attributeIndex){
        Relation output = Relation(input->name,input->header);
        output.header.deleteAttribute(attributeIndex);
        //I need to make a new set of tuples that I will fill after I remove the attribute at index.
        std::set<Tuple> newTuples;
        std::set<Tuple>::iterator it;
        for ( it = input->tuples.begin(); it != input->tuples.end(); ++it){
            Tuple tempTuple = *it;
            std::vector<std::string> newTuple = tempTuple.getValues();
            newTuple.erase(newTuple.begin()+attributeIndex);
            newTuples.insert(newTuple);
        }
        //I need to add these tuples to the output tuple.
        output.tuples = newTuples;

        return output;
    }

    Relation rename(Relation* input, int attributeIndex, std::string attribute){
        //This takes in the new name for the attribute;
        Relation output = Relation(input->name, input->header,input->tuples);
        output.header.rename(attributeIndex,attribute);
        return output;

    }

    Relation selectTwo(Relation* input, int attributeIndex1, int attributeIndex2 ){
        Relation output = Relation(input->name,input->header);//I'm just keeping the name

        std::set<Tuple>::iterator it;
        std::vector<std::set<Tuple>::iterator> iterators;
        std::set<Tuple> newTuples;

        for( it = input->tuples.begin(); it != input->tuples.end(); ++it){
            Tuple currentTuple = *it;
            if (currentTuple.valueAtI(attributeIndex1) != currentTuple.valueAtI(attributeIndex2)){
                //If there are not equivelant then we want to delete them. So we add them to the list of things to delete.
                iterators.push_back(it);
            }
            else if (currentTuple.valueAtI(attributeIndex1) == currentTuple.valueAtI(attributeIndex2)){
                //Do nothing.
            }
        }
        //Now we need to delete all the tuples where there is no match at the right attribute.
        for(unsigned int i = 0; i < iterators.size();i++){
            input->tuples.erase(iterators.at(i));
        }
        output.tuples = input->tuples;
        return output;
    }

    std::string result(){
        std::stringstream out;
        unsigned int numRows = tuples.size();
        if(!numRows){
            out<< "No\n";
        }
        else{
            out << "Yes(" <<numRows << ")\n";
        }

        return out.str();
    }

    std::string toString() const
    {

        //First check if there are any attributes left.
        if(!header.numAttributes()){
            return "";
        }
        std::stringstream out;

        //The tuples are the rows, the attributes are the columns.
        for(auto  it = tuples.begin(); it != tuples.end(); ++it){
            out <<"  ";
            for (unsigned int j = 0; j < header.numAttributes(); j++){
                out<< header.attributeAtI(j) <<"= "<< it->valueAtI(j);
                if(j < header.numAttributes()-1){
                    out <<", ";
                }

            }
            out<<"\n";

        }


        return out.str();
    }

    friend std::ostream& operator<< (std::ostream& os,  Relation& myclass)
    {
        os << myclass.toString();
        return os;
    }
};



#endif //CS236_PROJECT1_RELATION_H

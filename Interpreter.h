
#ifndef CS236_PROJECT1_INTERPRETER_H
#define CS236_PROJECT1_INTERPRETER_H



#include "DatalogProgram.h"
#include "Database.h"

#include "StringInts.h"
#include "KeyValues.h"


class Interpreter{

private:
    Database database;
    DatalogProgram datalogProgram;
    std::stringstream outString;

public:
    Interpreter(DatalogProgram datalogProgram){
        this->datalogProgram = datalogProgram;

        // Make a relation for each scheme-predicate, and put that in the database.
        for (unsigned int i = 0; i < datalogProgram.numSchemes(); i++){

            std::string name = datalogProgram.schemeIDAtI(i);
            Header header;
            for (unsigned int j = 0; j < datalogProgram.schemeSizeAtI(i); j++){
                std::string attribute = datalogProgram.getSchemeParamAtIJ(i,j);
                header.addAttribute(attribute);
            }
            //Now we make the relation
            Relation relation = Relation(name,header);
            //Add the relation to the database.
            database.addRelation(name,relation);
        }

        //Make a tuple for each fact predicate and add it to the appropriate relation in the database.
        for(unsigned int i = 0; i< datalogProgram.numFacts(); i++){
            Tuple tuple;
            std::string name = datalogProgram.factIDAtI(i);

            for (unsigned int j = 0; j  < datalogProgram.factSizeAtI(i);j++){
                std::string param = datalogProgram.getFactParamAtIJ(i,j);
                tuple.addParam(param);
            }

            database.addTuple(name, tuple);
        }
    }

    std::pair<int,int> repeatedAttributes(Relation* relation){
        std::vector<std::string> attributes = relation->getAttributes();
        for(unsigned int i = 0; i < attributes.size(); i++){
            for(unsigned int j = 0; j< attributes.size();j++){
                if(attributes.at(i) == attributes.at(j) && i != j){
                    return std::make_pair(i,j);
                }
            }
        }
        //Now that we have gone through the whole thing, we return true
        return std::make_pair(-1,-1);
    }

    Relation evalQuery (Predicate predicate){
        //NOTE: Somewhere here I erase the name of the relation, but that seems to be fine.
        std::string ID = predicate.getID();

        Relation output = database.getRelation(ID);

        //I need to find which params in my queries are constants and which are variables
        std::vector<std::pair<int,std::string>> constants;
        std::vector<std::pair<int,std::string>> variables;


        //Search for every constant in the query and store them in a vector of pairs.
        for (unsigned int i = 0; i<predicate.getNumParam(); i++){
            std::string param = predicate.getParamAtI((i));//For debugging.
            if(predicate.isParamAtIConst(i) == true){
                constants.push_back(make_pair(i, param));
            }
            else{
                variables.push_back(make_pair(i,param));
            }
        }

        //SELECT TYPE ONE
        //Now I need to do the select on the relation for however many constants I have.
        for(unsigned int i = 0; i < constants.size();i++){
            //The attributeIndex is the first in the pair. The string is the second
            output = output.select(&output, constants[i].first,constants[i].second);


        }

        //Now I need to do projection and remove each of the columns with constants
        //I need to erase the indices in reverse order.
        for(int i = constants.size()-1; i >= 0;i--){
            //output.project(constants[i].first);
            output = output.project(&output,constants[i].first);
        }




        //Now we do the rename for all the variables

        //Before we do the rename for all the variables, we have deleted columns so our variables vector is out of order
        for(unsigned int i = 0; i< variables.size();i++){
            output = output.rename(&output, i, variables[i].second);
            //Instead of passing in the index of where the variables appear on the predicate, we will pass the index of the actual vector that holds the variables.
        }





        //Now we need to do select type two for all the repeats
        while (repeatedAttributes(&output).first != -1){
            //Attributes at indeces i and j are the same so we need to do a type two select on them.
            auto ij = repeatedAttributes(&output);
            output = output.selectTwo(&output, ij.first,ij.second);

            //After we delete the rows that don't have matching values at the attributes, we need to delete the second, matching attribute column.
            output = output.project(&output,ij.second);
        }

        //I'm going to restore the name here instead of find where it went missing
        output.setName(ID);
        return output;
    }

    void evalQueries(){

        for(unsigned int i = 0; i < datalogProgram.numQueries(); i++){
            Relation evaluated = evalQuery(datalogProgram.queryAtI(i));
            std::string query = datalogProgram.queryAtI(i).toString() + "? ";//This was the original query. I added a space so the result can be pinned right after
            std::string queryResult = evaluated.result();
            std::string relation = evaluated.toString();
            outString << query << queryResult << relation;
            int a = 3;

        }
       // int a = datalogProgram.numQueries();
    }



    std::string toString() const
    {
        return outString.str();
    }

    friend std::ostream& operator<< (std::ostream& os,  Interpreter& myclass)
    {
        os << myclass.toString();
        return os;
    }
};




#endif //CS236_PROJECT1_INTERPRETER_H

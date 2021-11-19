
#ifndef CS236_PROJECT1_HEADER_H
#define CS236_PROJECT1_HEADER_H



class Header{



private:
    std:: vector<std::string> attributes;

public:
    Header(){}

    void addAttribute(std::string attribute){
        attributes.push_back(attribute);
    }

    void deleteAttribute(int index){

        attributes.erase(attributes.begin()+index);
    }
    void rename(int index, std::string newName){
        attributes[index] = newName;
    }
    std::vector<std::string> getAttributes(){
        return attributes;
    }
    unsigned int numAttributes() const {
        return attributes.size();
    }
    std::string attributeAtI(int index) const{
        return attributes.at(index);
    }
};
#endif //CS236_PROJECT1_HEADER_H

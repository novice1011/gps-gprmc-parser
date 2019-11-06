#include <string>
#include <iostream>

class GPRMC{
public:
    bool translate(std::string wordinput){ //creation
        //get checksum
        int starpos = wordinput.find('*');
        int checksum = std::stoi( wordinput.substr(starpos+1),NULL,16 );//16 as radix, means that the string is hex, we conv it to int
        std::string word = wordinput.substr(1,starpos-1);
        //calculate
        int tmp = word[0];
        for (int i=1;i<word.size();i++) {
            tmp = tmp^word[i];
        }
        if (checksum!=tmp){
            possible=0;
            return 0;
        }

        //find index
        indexseparator[0]=wordinput.find(separator);
        for (int i = 1; i<separatornum; i++){
            indexseparator[i]=wordinput.find(",",indexseparator[i-1]+1);
        }
        //seperate
        substringout[0] = wordinput.substr(1, indexseparator[0]-1);
        for (int i=1; i<=12; i++) {
            substringout[i] = wordinput.substr(indexseparator[i-1]+1, indexseparator[i]-indexseparator[i-1]-1);
        }
        possible = 1;
        return 1;
    }
    std::string getSubstring(int index){
        if(!possible)return 0;
        return substringout[index];
    }
    double getLat(){
        if(!possible)return 0;
        if (substringout[3]==""||substringout[4]=="")return 0;
        double tmpf = std::stod(substringout[3]);
        double dd = (int)tmpf / 100;
        double mm = tmpf-dd*100;
        double dd2 = mm/60;
        double result = dd+dd2;
        if (substringout[4]=="S") return result*-1;
        return result;
    }
    double getLong(){
        if(!possible)return 0;
        if (substringout[5]==""||substringout[6]=="")return 0;
        double tmpf = std::stod(substringout[5]);
        double dd = (int)tmpf / 100;
        double mm = tmpf-dd*100;
        double dd2 = mm/60;
        double result = dd+dd2;
        if (substringout[6]=="W") return result*-1;
        return result;
    }
    float getSpeed(){
        if(!possible)return 0;
        if (substringout[7]=="")return 0;
        double speed = std::stod(substringout[7]);
        return speed;
    }
    float getCourse(){
        if(!possible)return 0;
        if (substringout[8]=="")return 0;
        double cog = std::stod(substringout[8]);
        return cog;
    }
private:
    bool possible=0;
    int separatornum = 12;
    char separator = ',';
    int indexseparator[12];
    std::string wordinput;
    std::string substringout[13];
};

void findseparatorindex(int separatornum, char separator, int *indexarray, std::string wordinput, std::string *substringout);

int main(int argc, char *argv[])
{
    std::string example = "$GPRMC,134020.00,A,2500.71511,N,12132.47267,E,0.069,,241019,,,A*73\n";//NMEA string
    std::string example2 = "$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\n";
    
    int comma_pos[12];
    
    std::string subexample[13];

    //find separator index
    findseparatorindex(12, ',', comma_pos, example, subexample);

    //print indexes
    for (int i = 0; i<12; i++){
        std::cout<<comma_pos[i]<<'\t'<<subexample[i]<<'\n';
    }

    GPRMC in;
    in.translate(example);
    std::cout<<in.translate(example)<<'\n';
    std::cout<<std::to_string(in.getLat())<<'\n';
    std::cout<<std::to_string(in.getLong())<<'\n';
    std::cout<<std::to_string(in.getSpeed())<<'\n';
    std::cout<<std::to_string(in.getCourse())<<'\n';
    std::cout<<"exit\n";
}

void findseparatorindex(int separatornum, char separator, int *indexarray, std::string wordinput, std::string *substringout){
    //find index
    indexarray[0]=wordinput.find(separator);
    for (int i = 1; i<separatornum; i++){
        indexarray[i]=wordinput.find(",",indexarray[i-1]+1);
    }
    //seperate
    substringout[0] = wordinput.substr(1, indexarray[0]-1);
    for (int i=1; i<=12; i++) {
        substringout[i] = wordinput.substr(indexarray[i-1]+1, indexarray[i]-indexarray[i-1]-1);
    }
}

//REFF:
//https://www.u-blox.com/sites/default/files/products/documents/u-blox6_ReceiverDescrProtSpec_%28GPS.G6-SW-10018%29_Public.pdf?utm_source=en%2Fimages%2Fdownloads%2FProduct_Docs%2Fu-blox6_ReceiverDescriptionProtocolSpec_%28GPS.G6-SW-10018%29.pdf
//HEX to INT : http://www.cplusplus.com/reference/cstdlib/strtol/

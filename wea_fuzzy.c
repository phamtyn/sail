term MAX_TEMP(-15, 50, 14)
{
    fuzzy freeze = -15;
    fuzzy cold = 5;
    fuzzy mild = 20;
    fuzzy hot = 35;
    fuzzy heat = 50;
};

term MIN_TEMP(-15, 50, 14)
{
    fuzzy freeze = -15;
    fuzzy cold = 5;
    fuzzy mild = 20;
    fuzzy hot = 35;
    fuzzy heat = 50;
};

term STORM(0, 10, 11)
{
    fuzzy violent = 10;
    fuzzy strong = 8;
    fuzzy medium = 6;
    fuzzy light = 3;
    fuzzy none = 0;
};

term WIND(0, 100, 11)
{
    fuzzy gale = 100;
    fuzzy strong = 60;
    fuzzy moderate = 40;
    fuzzy breeze = 20;
    fuzzy calm = 0;
};

term THUNDER(0, 1, 4)
{
    fuzzy yes = 1;
    fuzzy no = 0;
};

term FOG(0, 1, 4)
{
    fuzzy yes = 1;
    fuzzy no = 0;
};

term WEATHER(0, 3, 4) {
    
    MAX_TEMP max_temp;
    MIN_TEMP min_temp;
    STORM storm;
    WIND wind;
    THUNDER thunder;
    FOG fog;
    
    fuzzy good = 0;
    fuzzy normal = 1;
    fuzzy bad = 2;
    fuzzy dangerous = 3;

    void Read(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg) {

        max_temp = max_temp_arg;
        min_temp = min_temp_arg;
        storm = storm_arg;
        wind = wind_arg;
        thunder = thunder_arg;
        fog = fog_arg;
    }

    void SetRules() {
        
        ZeroFuzz(this);
        
        rule max_temp.heat OR LITTLE min_temp.freeze => dangerous;
        rule LITTLE LITTLE storm.violent OR BIT storm.strong OR wind.gale => dangerous;
        rule (REALLY wind.strong OR REALLY storm.medium) AND
            EXTREMELY NOT (max_temp.heat OR LITTLE min_temp.freeze) => bad;
            
        rule thunder.yes AND (wind.moderate OR storm.light) AND 
            EXTREMELY NOT (max_temp.heat OR LITTLE min_temp.freeze) => bad;
            
        rule SEEMED max_temp.mild AND SEEMED min_temp.mild AND storm.none AND
            (LITTLE LITTLE wind.calm OR BIT wind.breeze) AND
            thunder.yes AND fog.no => normal;    
            
        rule REALLY fog.yes AND EXTREMELY NOT (max_temp.heat OR LITTLE min_temp.freeze) AND
            VERY VERY VERY NOT (LITTLE LITTLE storm.violent OR BIT storm.strong OR
            LITTLE wind.gale) => bad;
            
        rule (max_temp.hot OR min_temp.cold) AND storm.none AND
            (LITTLE LITTLE wind.calm OR BIT wind.breeze) AND
            thunder.no AND fog.no => normal;
            
        rule SEEMED max_temp.mild AND SEEMED min_temp.mild AND storm.light AND
            (LITTLE LITTLE wind.calm OR BIT wind.breeze) AND
            thunder.no AND fog.no => normal;
        
        rule max_temp.mild AND min_temp.mild AND storm.none AND
            wind.moderate AND thunder.no AND fog.no => normal;
        
        rule LITTLE LITTLE max_temp.mild AND LITTLE LITTLE min_temp.mild AND
            storm.none AND (LITTLE LITTLE wind.calm OR BIT wind.breeze) AND
            thunder.no AND fog.no => good;
    }

    void Process(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg) {
        
        Read(max_temp_arg, min_temp_arg, storm_arg, wind_arg, thunder_arg, fog_arg);
        SetRules();
        Display();
    }
    
    String int2string(int code) {
        switch (code) {
            case 0:
                return "Good";
            case 1:
                return "Normal";
            case 2:
                return "Bad";
            case 3:
                return "Danger";
        }
    }

    void Display() {
    
        print int2string(Classify(this));
    }
    
};

WEATHER weather;
int count = system_reading_count;
if (count != 6) {
    print "Invalid reading count!";
    exit(-1);
}
Number args[count];
for (int i = 0; i < count; i++)
    args[i] = system_reading_array[i];
weather.Process(args[0], args[1], args[2], args[3], args[4], args[5]);

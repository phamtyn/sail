term MAX_TEMP(12)
{
    fuzzy discrete freeze(1/-10, 1/-5, 0.75/0, 0.5/5, 0.25/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
    fuzzy discrete cold(0/-10, 0.25/-5, 0.5/0, 0.75/5, 1/10, 0.75/15, 0.5/20, 0.25/25, 0/30, 0/35, 0/40, 0/45);
    fuzzy discrete mild(0/-10, 0/-5, 0/0, 0/5, 0/10, 0.25/15, 0.5/20, 1/25, 0.5/30, 0.25/35, 0/40, 0/45);
    fuzzy discrete hot(0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0.25/25, 0.5/30, 1/35, 0.5/40, 0.25/45);
    fuzzy discrete heat(0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0.25/35, 0.5/40, 1/45);
};

term MIN_TEMP(12)
{
    fuzzy discrete freeze(1/-10, 1/-5, 0.75/0, 0.5/5, 0.25/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
    fuzzy discrete cold(0/-10, 0.25/-5, 0.5/0, 0.75/5, 1/10, 0.75/15, 0.5/20, 0.25/25, 0/30, 0/35, 0/40, 0/45);
    fuzzy discrete mild(0/-10, 0/-5, 0/0, 0/5, 0/10, 0.25/15, 0.5/20, 1/25, 0.5/30, 0.25/35, 0/40, 0/45);
    fuzzy discrete hot(0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0.25/25, 0.5/30, 1/35, 0.5/40, 0.25/45);
    fuzzy discrete heat(0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0.25/35, 0.5/40, 1/45);
};

term STORM(11)
{
    fuzzy discrete strong(0/0, 0/1, 0/2, 0/3, 0/4, 0.25/5, 0.5/6, 0.75/7, 0.75/8, 1/9, 1/10);
    fuzzy discrete light(0/0, 0.25/1, 0.5/2, 1/3, 0.5/4, 0.25/5, 0/6, 0/7, 0/8, 0/9, 0/10);
    fuzzy discrete none(1/0, 0.25/1, 0/2, 0/3, 0/4, 0/5, 0/6, 0/7, 0/8, 0/9, 0/10);
};

term WIND(11)
{
    fuzzy discrete gale(0/0, 0/10, 0/20, 0/30, 0/40, 0/50, 0/60, 0.25/70, 0.5/80, 0.75/90, 1/100);
    fuzzy discrete strong(0/0, 0/10, 0/20, 0/30, 0.25/40, 0.5/50, 1/60, 0.5/70, 0.25/80, 0/90, 0/100);
    fuzzy discrete moderate(0/0, 0/10, 0.25/20, 0.5/30, 1/40, 0.5/50, 0.25/60, 0/70, 0/80, 0/90, 0/100);
    fuzzy discrete breeze(0.25/0, 0.5/10, 1/20, 0.5/30, 0.25/40, 0/50, 0/60, 0/70, 0/80, 0/90, 0/100);
    fuzzy discrete calm(1/0, 0.5/10, 0.25/20, 0/30, 0/40, 0/50, 0/60, 0/70, 0/80, 0/90, 0/100);
};

term THUNDER(3)
{
    fuzzy discrete yes(0/0, 0.5/0.5, 1/1);
    fuzzy discrete no(1/0, 0.5/0.5, 0/1);
};

term FOG(3)
{
    fuzzy discrete yes(0/0, 0.5/0.5, 1/1);
    fuzzy discrete no(1/0, 0.5/0.5, 0/1);
};

term SCORE(4)
{
    fuzzy discrete good(1/0, 0.5/1, 0/2, 0/3);
    fuzzy discrete normal(0.5/0, 1/1, 0.5/2, 0/3);
    fuzzy discrete bad(0/0, 0.5/1, 1/2, 0.5/3);
    fuzzy discrete dangerous(0/0, 0/1, 0.5/2, 1/3);
};

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

struct WEATHER {
    
    MAX_TEMP max_temp;
    MIN_TEMP min_temp;
    STORM storm;
    WIND wind;
    THUNDER thunder;
    FOG fog;
    SCORE score;

    void Read(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg) {

        PutReading (max_temp, max_temp_arg);
        PutReading (min_temp, min_temp_arg);
        PutReading (storm, storm_arg);
        PutReading (wind, wind_arg);
        PutReading (thunder, thunder_arg);
        PutReading (fog, fog_arg);
    }

    void SetRules() {
        
        ZeroFuzz(score);

        rule max_temp.heat OR min_temp.freeze => score.dangerous;
        rule storm.strong OR wind.gale => score.dangerous;
        rule wind.strong OR storm.light => score.bad;
        rule wind.moderate AND thunder.yes => score.bad;
        rule VERY fog.yes => score.bad;
        rule (max_temp.hot OR min_temp.cold) AND storm.none AND wind.moderate AND
            thunder.no AND fog.no => score.normal;
        rule LITTLE max_temp.mild AND LITTLE min_temp.mild AND REALLY storm.none AND
            (wind.breeze OR wind.calm) AND thunder.no AND fog.no => score.good;
    }

    void Process(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg) {
        
        Read(max_temp_arg, min_temp_arg, storm_arg, wind_arg, thunder_arg, fog_arg);
        SetRules();
        Display();
    }
    
    int defuzz_score() {
        int num_score = score.size;
        FuzzyPair array_all[num_score];
        FuzzyToArray(score, array_all);
        int index_max_first = 0;
        Number max_grade = 0;
        for(int i = 0; i < num_score; i++)
            if (array_all[i].m_grade > max_grade) {
                max_grade = array_all[i].m_grade;
                index_max_first = i;
            }
        Number array_max[num_score];
        int count_max = 0;
        for(int i = index_max_first; i < num_score; i++)
            if (array_all[i].m_grade == max_grade) {
                array_max[count_max] = array_all[i].m_value;
                count_max++;
            }
        int value_max_average = floor( (array_max[0] + array_max[count_max - 1]) / 2 );
        return value_max_average;
    }

    void Display() {
        int score = defuzz_score();
        print int2string(score);
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

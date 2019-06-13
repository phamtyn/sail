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

int round_input(Number val, int interval) {

    Number quo = val / interval;

    int out = round(quo) * interval;

    return out;
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

struct WEATHER
{
    MAX_TEMP max_temp;
    MIN_TEMP min_temp;
    STORM storm;
    WIND wind;
    THUNDER thunder;
    FOG fog;
    SCORE score;

    bool Read(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg)
    {
        int i;

        switch ( i = round_input(max_temp_arg, 5) )
        {
        case -10:
            put max_temp := (1/i, 0.5/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case -5:
            put max_temp := (0.5/-10, 1/i, 0.5/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 0:
            put max_temp := (0/-10, 0.5/-5, 1/i, 0.5/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 5:
            put max_temp := (0/-10, 0/-5, 0.5/0, 1/i, 0.5/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 10:
            put max_temp := (0/-10, 0/-5, 0/0, 0.5/5, 1/i, 0.5/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 15:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0.5/10, 1/i, 0.5/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 20:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0.5/15, 1/i, 0.5/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 25:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0.5/20, 1/i, 0.5/30, 0/35, 0/40, 0/45);
            break;
        case 30:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0.5/25, 1/i, 0.5/35, 0/40, 0/45);
            break;
        case 35:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0.5/30, 1/i, 0.5/40, 0/45);
            break;
        case 40:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0.5/35, 1/i, 0.5/45);
            break;
        case 45:
            put max_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0.5/40, 1/i);
            break;
        default:
            return false;
        }
        
        switch ( i = round_input(min_temp_arg, 5) )
        {
        case -10:
            put min_temp := (1/i, 0.5/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case -5:
            put min_temp := (0.5/-10, 1/i, 0.5/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 0:
            put min_temp := (0/-10, 0.5/-5, 1/i, 0.5/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 5:
            put min_temp := (0/-10, 0/-5, 0.5/0, 1/i, 0.5/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 10:
            put min_temp := (0/-10, 0/-5, 0/0, 0.5/5, 1/i, 0.5/15, 0/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 15:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0.5/10, 1/i, 0.5/20, 0/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 20:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0.5/15, 1/i, 0.5/25, 0/30, 0/35, 0/40, 0/45);
            break;
        case 25:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0.5/20, 1/i, 0.5/30, 0/35, 0/40, 0/45);
            break;
        case 30:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0.5/25, 1/i, 0.5/35, 0/40, 0/45);
            break;
        case 35:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0.5/30, 1/i, 0.5/40, 0/45);
            break;
        case 40:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0.5/35, 1/i, 0.5/45);
            break;
        case 45:
            put min_temp := (0/-10, 0/-5, 0/0, 0/5, 0/10, 0/15, 0/20, 0/25, 0/30, 0/35, 0.5/40, 1/i);
            break;
        default:
            return false;
        }

        switch ( i = storm_arg )
        {
        case 0:
            put storm := (1/i, 0.5/1, 0/2, 0/3, 0/4, 0/5, 0/6, 0/7, 0/8, 0/9, 0/10);
            break;
        case 1:
            put storm := (0.5/0, 1/i, 0.5/2, 0/3, 0/4, 0/5, 0/6, 0/7, 0/8, 0/9, 0/10);
            break;
        case 2:
            put storm := (0/0, 0.5/1, 1/i, 0.5/3, 0/4, 0/5, 0/6, 0/7, 0/8, 0/9, 0/10);
            break;
        case 3:
            put storm := (0/0, 0/1, 0.5/2, 1/i, 0.5/4, 0/5, 0/6, 0/7, 0/8, 0/9, 0/10);
            break;
        case 4:
            put storm := (0/0, 0/1, 0/2, 0.5/3, 1/i, 0.5/5, 0/6, 0/7, 0/8, 0/9, 0/10);
            break;
        case 5:
            put storm := (0/0, 0/1, 0/2, 0/3, 0.5/4, 1/i, 0.5/6, 0/7, 0/8, 0/9, 0/10);
            break;
        case 6:
            put storm := (0/0, 0/1, 0/2, 0/3, 0/4, 0.5/5, 1/i, 0.5/7, 0/8, 0/9, 0/10);
            break;
        case 7:
            put storm := (0/0, 0/1, 0/2, 0/3, 0/4, 0/5, 0.5/6, 1/i, 0.5/8, 0/9, 0/10);
            break;
        case 8:
            put storm := (0/0, 0/1, 0/2, 0/3, 0/4, 0/5, 0/6, 0.5/7, 1/i, 0.5/9, 0/10);
            break;
        case 9:
            put storm := (0/0, 0/1, 0/2, 0/3, 0/4, 0/5, 0/6, 0/7, 0.5/8, 1/i, 0.5/10);
            break;
        case 10:
            put storm := (0/0, 0/1, 0/2, 0/3, 0/4, 0/5, 0/6, 0/7, 0/8, 0.5/9, 1/i);
            break;
        default:
            return false;
        }

        switch ( i = round_input(wind_arg, 10) )
        {
        case 0:
            put wind := (1/i, 0.5/10, 0/20, 0/30, 0/40, 0/50, 0/60, 0/70, 0/80, 0/90, 0/100);
            break;
        case 10:
            put wind := (0.5/0, 1/i, 0.5/20, 0/30, 0/40, 0/50, 0/60, 0/70, 0/80, 0/90, 0/100);
            break;
        case 20:
            put wind := (0/0, 0.5/10, 1/i, 0.5/30, 0/40, 0/50, 0/60, 0/70, 0/80, 0/90, 0/100);
            break;
        case 30:
            put wind := (0/0, 0/10, 0.5/20, 1/i, 0.5/40, 0/50, 0/60, 0/70, 0/80, 0/90, 0/100);
            break;
        case 40:
            put wind := (0/0, 0/10, 0/20, 0.5/30, 1/i, 0.5/50, 0/60, 0/70, 0/80, 0/90, 0/100);
            break;
        case 50:
            put wind := (0/0, 0/10, 0/20, 0/30, 0.5/40, 1/i, 0.5/60, 0/70, 0/80, 0/90, 0/100);
            break;
        case 60:
            put wind := (0/0, 0/10, 0/20, 0/30, 0/40, 0.5/50, 1/i, 0.5/70, 0/80, 0/90, 0/100);
            break;
        case 70:
            put wind := (0/0, 0/10, 0/20, 0/30, 0/40, 0/50, 0.5/60, 1/i, 0.5/80, 0/90, 0/100);
            break;
        case 80:
            put wind := (0/0, 0/10, 0/20, 0/30, 0/40, 0/50, 0/60, 0.5/70, 1/i, 0.5/90, 0/100);
            break;
        case 90:
            put wind := (0/0, 0/10, 0/20, 0/30, 0/40, 0/50, 0/60, 0/70, 0.5/80, 1/i, 0.5/100);
            break;
        case 100:
            put wind := (0/0, 0/10, 0/20, 0/30, 0/40, 0/50, 0/60, 0/70, 0/80, 0.5/90, 1/i);
            break;
        default:
            return false;
        }
        
        switch ( i = thunder_arg )
        {
        case 0:
            put thunder := (1/i, 0.5/0.5, 0/1);
            break;
        case 1:
            put thunder := (0/0, 0.5/0.5, 1/i);
            break;
        default:
            return false;
        }
        
        switch ( i = fog_arg )
        {
        case 0:
            put fog := (1/i, 0.5/0.5, 0/1);
            break;
        case 1:
            put fog := (0/0, 0.5/0.5, 1/i);
            break;
        default:
            return false;
        }

        return true;
    }

    void SetRules()
    {
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

    void Process(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg)
    {
        if ( Read(max_temp_arg, min_temp_arg, storm_arg, wind_arg, thunder_arg, fog_arg) )
        {
            SetRules();
            Display();
        }
        else
            print "Invalid input!\n";
    }
    
    int GetScoreCode(Number max_temp_arg, Number min_temp_arg, int storm_arg, Number wind_arg, int thunder_arg, int fog_arg)
    {
        if ( Read(max_temp_arg, min_temp_arg, storm_arg, wind_arg, thunder_arg, fog_arg) )
            return defuzz_score();
        else
            return -1;
    }
    
    int defuzz_score() {
        int num_score = 4;
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



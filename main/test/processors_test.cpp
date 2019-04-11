#define BOOST_TEST_MODULE Processor Unit Test

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/test/included/unit_test.hpp>

#include "../include/grid.h"
#include "../include/processors/processor.h"
#include "processors/processor_sn_st.h"
#include "processors/processor_sn_mt.h"
#include "processors/processor_mn_mt.h"

struct processor_tester final
{
    explicit processor_tester(processor & p) : p(p) {}

    void test_preprocess()
    {
        p.preprocess();
        BOOST_TEST(p.record == preprocess_tiny_ans);
    }

    void test_process()
    {
        BOOST_TEST(p() == process_tiny_ans);
    }

private:
    static processor::record_type const preprocess_tiny_ans;
    static processor::result_type const process_tiny_ans;

    processor & p;
};

struct GridFixture
{
    grid g;
};

struct MPIFixture : GridFixture
{
    explicit MPIFixture() = default;

    virtual ~MPIFixture() = default;

    boost::mpi::environment env;
    boost::mpi::communicator world;
};

BOOST_FIXTURE_TEST_SUITE(Processors, GridFixture)

BOOST_AUTO_TEST_CASE(SINGLE_THREAD)
{
    processor_sn_st p("tinyTwitter.json", g);
    processor_tester tester(p);

    tester.test_preprocess();
    tester.test_process();
}

BOOST_AUTO_TEST_CASE(MULTI_THREAD)
{
    processor_mt p("tinyTwitter.json", g);
    processor_tester tester(p);

    tester.test_preprocess();
    tester.test_process();
}

BOOST_FIXTURE_TEST_CASE(MULTI_NODE, MPIFixture)
{
    processor_mn_mt p(env, world, "tinyTwitter.json", g);
    processor_tester tester(p);

    tester.test_preprocess();
    tester.test_process();
}

BOOST_AUTO_TEST_SUITE_END()

// @formatter:off
processor::record_type const processor_tester::preprocess_tiny_ans {
    {
        "C3",
        {
            145,
            {
                {"linacreroad", 1},
                {"aflswansbombers", 1},
                {"qanda", 1},
                {"whenwillitend", 1},
                {"longlost", 1},
                {"hazmat", 1},
                {"mufc", 1},
                {"bernieshithebeach", 1},
                {"tvd", 1}, {"sandringham", 2},
                {"endomondo", 1},
                {"howsayingno", 1},
                {"bts15", 1},
                {"swimwithsharks", 1},
                {"superstar", 1},
                {"warner", 1},
                {"smoothies", 1},
                {"amoshair", 1},
                {"teamireland", 1},
                {"thatrainkissthough", 1},
                {"sundaysesh", 1},
                {"melbourne", 2},
                {"realstrength", 1},
                {"entertainment", 1},
                {"arias", 1},
                {"vidalifestyle", 1},
                {"socialmedia", 1},
                {"bigdata", 1},
                {"friday", 1},
                {"famlife14", 2},
                {"victraffic", 2},
                {"structurefire", 1},
                {"avfc", 3},
                {"tuesday", 1},
                {"strava", 1},
                {"hellenicrepublic", 1},
                {"febfast", 1},
                {"thankstony", 1},
                {"vafa", 1},
                {"bully", 1},
                {"wishiwasakidagain", 1},
                {"firealarm", 3},
                {"livethedream", 1},
                {"osca", 1},
                {"incident", 2},
                {"sandyzebras", 1},
                {"repost", 2},
                {"herewego", 1}
            }
        }
    },
    {
        "C2",
        {
            472,
            {
                {"gobernards", 1},
                {"sorrynotsorry", 1},
                {"hiit", 1},
                {"hazmat", 1},
                {"himym", 1},
                {"bunnings", 1},
                {"fitness", 1},
                {"jan1st", 1},
                {"roadtomelbourne", 1},
                {"misikepekajaqkeostolia", 1},
                {"lifeisgood", 1},
                {"whatdidiweartoday", 1},
                {"sea", 1},
                {"luckybamboo", 1},
                {"bringonsummer", 1},
                {"ichill", 1},
                {"comeonireland", 1},
                {"จะออกไปแตะขอบฟ้า", 1},
                {"gobernies", 1},
                {"summer", 2},
                {"hugoboss", 1},
                {"spotto", 1},
                {"superbowlxlix", 1},
                {"t20blast", 1},
                {"watchthegrouch", 1},
                {"gopro", 1},
                {"structurefire", 1},
                {"goodmorning", 1},
                {"selfemployedlife", 1},
                {"hellomoon", 1},
                {"instag_app", 1},
                {"highcountry", 1},
                {"melbournetobewild", 1},
                {"straya", 1},
                {"hot", 1},
                {"famlife14", 3},
                {"tasteofmelbourne", 1},
                {"fatalframes", 1},
                {"chanwontravel", 1},
                {"brightonbeach", 2},
                {"fullmoon", 1},
                {"beachmorning", 1},
                {"brightness", 1},
                {"wymtm", 1},
                {"cycling", 2},
                {"hounddog", 1},
                {"brideandgroom", 1},
                {"sevenstepswriting", 1},
                {"holidays", 1},
                {"victoria", 1},
                {"dog", 1},
                {"ootd", 1},
                {"weddingbants", 1},
                {"yachtlyf", 1},
                {"melborune", 2},
                {"beachdays", 1},
                {"goprohero3", 1},
                {"repost", 1},
                {"sandringhamyachtclub", 1},
                {"sunsets", 1},
                {"livinglife", 1},
                {"avenezuelanabroad", 1},
                {"sundays", 1},
                {"giantceltic", 1},
                {"bitchdrink", 1},
                {"wheresmymanners", 1},
                {"girls", 1},
                {"seeaustralia", 1},
                {"peterpan", 1},
                {"bread", 1},
                {"ausyachtingchamps", 1},
                {"sandringhambeach", 1},
                {"beachlifestyle", 3},
                {"incident", 1},
                {"storm", 2},
                {"eyefillet", 1},
                {"questionsformick", 3},
                {"challengemelbourne", 1},
                {"architecture", 3},
                {"clobynmania", 1},
                {"melbourne", 16},
                {"spiritoftasmania", 1},
                {"dogbeach", 1},
                {"brightonbathingboxes", 1},
                {"welcomebony", 1},
                {"tgif", 1},
                {"littlemelbourne", 2},
                {"vic", 1},
                {"demviews", 1},
                {"runryan", 1},
                {"confused", 1},
                {"melbourneiloveyou", 1},
                {"sandringham", 1},
                {"summerday", 1},
                {"happynewyear", 1},
                {"art", 1},
                {"coffee", 1},
                {"mylifedownunder2017", 1},
                {"nofilter", 3},
                {"latergram", 1},
                {"firealarm", 2},
                {"tb", 1},
                {"sunset_madness", 1},
                {"sunset", 5},
                {"sonandirene2015", 1},
                {"lovemygiant", 1},
                {"auskick", 1},
                {"beach", 4},
                {"ocean", 1},
                {"bayside", 1},
                {"hampton", 2},
                {"greatstaff", 1},
                {"messingaround", 1},
                {"lowtide", 1},
                {"heritagehalfwheelers", 1},
                {"akalainmo", 1},
                {"summeriscoming", 1},
                {"lovedit", 1},
                {"instaweather", 1},
                {"bikecruising", 1},
                {"bbl04", 1},
                {"olympic", 1},
                {"runhappysandy", 1},
                {"tradie", 1},
                {"wanderingsandco", 1},
                {"loveprohours", 1},
                {"walkingthedog", 1},
                {"sbs", 1},
                {"pb", 1},
                {"wanderlust", 1},
                {"dryneedlingusa", 1},
                {"brighton", 5},
                {"nylexclock", 1},
                {"australia", 6},
                {"victraffic", 3},
                {"westerlywordmatters", 1},
                {"bbq", 1},
                {"kitporn", 1},
                {"semiandcaleb", 4},
                {"challengemelb", 5},
                {"turnitup", 1},
                {"letsgobernies", 1},
                {"tabloids", 1},
                {"aroundtheworldwithnataliatangling", 1},
                {"skiesthroughoureyes", 1},
                {"australianlogic", 1},
                {"syc", 2},
                {"austuralia", 1},
                {"festiveride", 1},
                {"nonstructurefire", 1},
                {"cycle", 1}
            }
        }
    },
    {
        "D3",
        {
            378,
            {
                {"captainabbott", 1},
                {"trust", 1},
                {"kinkuji", 2},
                {"justlovely", 1},
                {"collectiveunitofnescafetraders", 1},
                {"volunteering", 1},
                {"eggallovermyface", 1},
                {"rockingrose", 1},
                {"primalliving", 1},
                {"nationalcoffeeday", 1},
                {"christmas", 1},
                {"max", 1},
                {"uncoolbutcool", 1},
                {"visitmelbourne", 1},
                {"люблюялето", 1},
                {"friends", 1},
                {"rocks", 1},
                {"vscocam", 1},
                {"architecture", 1},
                {"friday", 1},
                {"dayoff", 2},
                {"jerkchicken", 1},
                {"spearfishing", 1},
                {"mosaic", 1},
                {"thatsmelbourne", 1},
                {"baysidebuyersagent", 1},
                {"humpday", 1},
                {"christmasaustralia", 1},
                {"puffpastry", 1},
                {"melborune", 2},
                {"riderfitcc", 1},
                {"iot", 1},
                {"mustwatch", 1},
                {"plugga", 1},
                {"estatemaster", 1},
                {"itsabitwarm", 1},
                {"noidea", 1},
                {"food", 1},
                {"cloud", 1},
                {"smile", 1},
                {"lookout", 1},
                {"neverstopexploring", 1},
                {"dashit", 1},
                {"holidays", 1},
                {"tonkinesecats", 1},
                {"celebchrissie", 1},
                {"sandringham", 7},
                {"butfirstcoffee", 1},
                {"swimwear", 1},
                {"halfmoonbay", 3},
                {"kulanikinis", 1},
                {"sand", 1},
                {"endomondo", 3},
                {"nonstructurefire", 4},
                {"spring", 1},
                {"outsideisfree", 3},
                {"blackrock", 2},
                {"fyxo", 1},
                {"fridaynight", 1},
                {"beachlife", 1},
                {"qldbesties", 1},
                {"bayside", 1},
                {"merrychristmas", 1},
                {"ouch", 1},
                {"drinklocal", 1},
                {"melbourne", 11},
                {"summer", 3},
                {"pinhead", 1},
                {"brighton", 1},
                {"govictory", 1},
                {"beach", 5},
                {"snakes", 1},
                {"incident", 2},
                {"victraffic", 7},
                {"data", 1},
                {"saturday", 1},
                {"pizza", 1},
                {"china", 1},
                {"structure", 1},
                {"microsoft", 1},
                {"chocolate", 1},
                {"waste", 1},
                {"thatkindaday", 1},
                {"thingsthatshitjulia", 2},
                {"love", 1},
                {"nonstructure", 1},
                {"dontpanic", 1},
                {"barney", 1},
                {"women", 1},
                {"sundaymorning", 1},
                {"latergram", 1},
                {"bleak", 1},
                {"vibes", 1},
                {"firealarm", 2},
                {"thisface", 1},
                {"fitness", 1},
                {"selflovesunday", 1},
                {"sickie", 1},
                {"sunset", 2},
                {"saturdaynight", 1},
                {"walkingthedog", 1},
                {"hehasmyeyes", 1},
                {"warm", 1},
                {"willmissyou", 1},
                {"sandridgebeach", 1},
                {"happynewyear", 1},
                {"coffee", 1},
                {"crazyweather", 1},
                {"boxingday", 1},
                {"run", 1},
                {"japan", 1},
                {"goodbyewinter", 1},
                {"structurefire", 2},
                {"liketkit", 1},
                {"gotiges", 1},
                {"tewinmelborune", 1}
            }
        }
    }
};
processor::result_type const processor_tester::process_tiny_ans {
    {
        "C2", 472, {
            {"melbourne", 16},
            {"australia", 6},
            {"brighton", 5},
            {"challengemelb", 5},
            {"sunset", 5}
        }
    },
    {
        "D3", 378, {
            {"melbourne", 11},
            {"sandringham", 7},
            {"victraffic", 7},
            {"beach", 5},
            {"nonstructurefire", 4}
        }
    },
    {
        "C3", 145, {
            {"avfc", 3},
            {"firealarm", 3},
            {"famlife14", 2},
            {"incident", 2},
            {"melbourne", 2}
        }
    }
};
// @formatter:on

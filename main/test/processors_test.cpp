#define BOOST_TEST_MODULE Processor Unit Test

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "grid.h"
#include "processors/processor.h"
#include "processors/processor_sn_st.h"
#include "processors/processor_sn_mt.h"

struct processor_tester final
{
    explicit processor_tester(processor & p) : p(p) {}

    void test_preprocess()
    {
        p.preprocess();
        BOOST_REQUIRE(p.record == preprocess_tiny_ans);
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

BOOST_FIXTURE_TEST_SUITE(Processors, GridFixture)

BOOST_AUTO_TEST_CASE(SNST)
{
    processor_sn_st p("tinyTwitter.json", g);
    processor_tester tester(p);

    tester.test_preprocess();
    tester.test_process();
}

BOOST_AUTO_TEST_CASE(SNMT)
{
    processor_sn_mt p("tinyTwitter.json", g);
    processor_tester tester(p);

    tester.test_preprocess();
    tester.test_process();
}

BOOST_AUTO_TEST_SUITE_END()

// @formatter:off
processor::record_type const processor_tester::preprocess_tiny_ans {
    {0, {}},
    {0, {}},
    {0, {}},
    {0, {}},
    {0, {}},
    {0, {}},
    {0, {}},
    {0, {}},
    {0, {}},
    {
        472,
        {
            {"separation", 1},
            {"schools", 1},
            {"vulnerable", 1},
            {"famlife14", 3},
            {"irs2014", 1},
            {"gobernards", 1},
            {"sorrynotsorry", 1},
            {"porkpie", 1},
            {"feta", 1},
            {"scotchegg", 1},
            {"bread", 1},
            {"littlemelbourne", 2},
            {"hiit", 1},
            {"tabloids", 1},
            {"melbournemoment", 1},
            {"avenezuelanabroad", 1},
            {"melbournebeaches", 1},
            {"walk", 1},
            {"youngpeople", 1},
            {"exercise", 1},
            {"day", 2},
            {"bitchdrink", 1},
            {"merceractive", 1},
            {"aiquebadalo", 1},
            {"challengemelbourne", 1},
            {"beachdays", 1},
            {"sun", 3},
            {"summeriscoming", 2},
            {"seeaustralia", 1},
            {"messingaround", 1},
            {"skiesthroughoureyes", 1},
            {"shalomsummer", 1},
            {"silhouette", 1},
            {"westerlywordmatters", 1},
            {"isaf", 1},
            {"incident", 1},
            {"sunrise", 1},
            {"lovedit", 1},
            {"wedding", 1},
            {"weddingbants", 1},
            {"melbourne_insta", 1},
            {"ausyachtingchamps", 1},
            {"latergram", 3},
            {"lifesabeach", 1},
            {"akalainmo", 1},
            {"community", 1},
            {"sunset_pics", 1},
            {"martinanddennise", 1},
            {"date", 2},
            {"love", 1},
            {"beers", 1},
            {"watchthegrouch", 1},
            {"festiveride", 1},
            {"art", 1},
            {"celebration", 1},
            {"botanicsmelbourne", 4},
            {"carrot", 1},
            {"buildings", 2},
            {"trek", 1},
            {"city", 4},
            {"brideandgroom", 1},
            {"igersmelbourne", 1},
            {"victorianelection", 1},
            {"sky", 1},
            {"potato", 1},
            {"dessert", 1},
            {"weekends", 1},
            {"sandringhambreakwater", 1},
            {"jenmcveity", 1},
            {"instawed", 1},
            {"sevenstepswriting", 1},
            {"summerday", 1},
            {"architecture", 3},
            {"wintersun", 1},
            {"yacht", 1},
            {"melbournebayside", 1},
            {"boatythings", 1},
            {"delicious", 1},
            {"beachboy", 1},
            {"pannacotta", 1},
            {"writing", 1},
            {"giantsouthyarra", 1},
            {"melborune", 2},
            {"australia", 15},
            {"sunsetbeach", 1},
            {"adventure", 1},
            {"nye", 1},
            {"lowtide", 1},
            {"mylifedownunder2017", 1},
            {"congrats", 1},
            {"wanderlust", 1},
            {"summerdreaming", 1},
            {"deserted", 1},
            {"comeonireland", 1},
            {"garmin", 1},
            {"bringtonbeach", 1},
            {"hounddog", 1},
            {"urbanlife", 1},
            {"goproのある生活", 2},
            {"sonandirene2015", 1},
            {"sunset", 15},
            {"instaweather", 1},
            {"lovers", 1},
            {"firealarm", 2},
            {"urban", 2},
            {"tasteofmelbourne", 1},
            {"beachhuts", 1},
            {"hypfocus", 1},
            {"sundaysession", 1},
            {"melbournelife", 6},
            {"pb", 1},
            {"aussiesofinstagram", 1},
            {"ettiquette", 1},
            {"summernights", 1},
            {"victoria", 5},
            {"vic", 5},
            {"lovemelbourne", 1},
            {"sandringhambeach", 2},
            {"beachlife", 3},
            {"newpens", 1},
            {"gegirltravel", 1},
            {"hound", 1},
            {"bluemoon", 1},
            {"tastingplate", 1},
            {"bayside", 2},
            {"nonstructurefire", 1},
            {"yachtlyf", 1},
            {"spiritoftasmania", 1},
            {"briiiiiiiiiiiiiighton", 1},
            {"union", 1},
            {"loveprohours", 1},
            {"clobynmania", 1},
            {"melbourne", 54},
            {"syc", 2},
            {"tasar", 1},
            {"type", 1},
            {"summer", 10},
            {"questionsformick", 3},
            {"socialgood", 1},
            {"voxfilms", 1},
            {"followchanwonto", 1},
            {"spring", 1},
            {"tasmania", 1},
            {"collabforimpact", 1},
            {"fatalframes", 1},
            {"sunsetbeers", 1},
            {"children", 1},
            {"goprohero3", 1},
            {"beachlifestyle", 3},
            {"ocean", 5},
            {"olympic", 1},
            {"beach", 22},
            {"citykillerz", 1},
            {"teachingliteracy", 1},
            {"aussie", 2},
            {"auskick", 1},
            {"i", 1},
            {"yummy", 1},
            {"corporatelunch", 1},
            {"baysidemelbourne", 1},
            {"madmax", 1},
            {"me", 1},
            {"turnitup", 1},
            {"tradie", 1},
            {"sbs", 1},
            {"bridaldance", 1},
            {"girls", 1},
            {"sunsetsky", 2},
            {"outsideisfree", 1},
            {"straya", 2},
            {"luckybamboo", 1},
            {"blackandwhite", 1},
            {"runhappysandy", 1},
            {"party", 1},
            {"brightonbathingboxes", 1},
            {"steak", 1},
            {"skyline", 1},
            {"giantceltic", 1},
            {"wymtm", 2},
            {"beautiful", 1},
            {"australianlogic", 1},
            {"fotosingh", 3},
            {"painting", 1},
            {"holidays", 1},
            {"beachlyf", 1},
            {"awesome", 2},
            {"reclaimthecold", 1},
            {"nofilter", 5},
            {"livinglife", 1},
            {"sunset_madness", 1},
            {"parents", 1},
            {"tb", 1},
            {"weather", 3},
            {"melbmoment", 1},
            {"landscape", 1},
            {"architexture", 3},
            {"blood", 1},
            {"sandringham", 6},
            {"melbournephotographer", 1},
            {"gauchasnaaustralia", 1},
            {"wx", 1},
            {"dog", 2},
            {"vicvotes", 1},
            {"sunsets", 4},
            {"lifeinmelbourne2017", 1},
            {"confused", 1},
            {"goose", 1},
            {"sailing", 2},
            {"aussie_images", 1},
            {"samphire", 1},
            {"austuralia", 1},
            {"numinstraya2017", 1},
            {"tgif", 1},
            {"roasted", 1},
            {"eyefillet", 1},
            {"yum", 1},
            {"storm", 2},
            {"skyscraper", 2},
            {"helterskelter", 1},
            {"wanderingsandco", 1},
            {"dryneedlingusa", 1},
            {"bathingboxes", 2},
            {"alieninvasion", 1},
            {"nylexclock", 1},
            {"7newsmelb", 1},
            {"rayban", 1},
            {"lovewhereyoulive", 1},
            {"sandringhamyachtclub", 1},
            {"instag_app", 2},
            {"triathlon", 3},
            {"clouds", 1},
            {"wheresmymanners", 1},
            {"victraffic", 3},
            {"spotto", 1},
            {"ゴープロのある生活", 2},
            {"melbourneiloveyou", 3},
            {"bbq", 1},
            {"moon", 1},
            {"ichill", 1},
            {"welcomebony", 1},
            {"bbl04", 1},
            {"glamour", 1},
            {"aroundtheworldwithnataliatangling", 1},
            {"loco", 1},
            {"views", 1},
            {"photography", 1},
            {"travels", 1},
            {"melbournethisway", 1},
            {"brightonbeach", 10},
            {"cute", 2},
            {"kitporn", 1},
            {"strava", 1},
            {"evening", 1},
            {"t20blast", 1},
            {"lovegoodtraining", 1},
            {"himym", 1},
            {"lifeisgood", 1},
            {"greatstaff", 1},
            {"cycling", 2},
            {"beachmorning", 1},
            {"heritagehalfwheelers", 1},
            {"fullmoon", 1},
            {"bloodmoon", 1},
            {"toecutter", 1},
            {"hazmat", 1},
            {"moonlight", 1},
            {"hot", 2},
            {"swimming", 1},
            {"seagull", 1},
            {"letsgobernies", 1},
            {"way2ill", 1},
            {"yngkillers", 1},
            {"nautical", 1},
            {"brightness", 1},
            {"illgrammers", 1},
            {"brighton", 12},
            {"sunsets_captures", 1},
            {"sand", 1},
            {"sando", 1},
            {"sweets", 1},
            {"sunkissed", 1},
            {"hat", 1},
            {"goldcoast", 2},
            {"gopro", 4},
            {"goprohero5", 2},
            {"instaweatherpro", 1},
            {"写真", 2},
            {"semiandcaleb", 4},
            {"skylovers", 1},
            {"iphoneography", 1},
            {"melbournetobewild", 1},
            {"hellomoon", 1},
            {"instagood", 1},
            {"allsmiles", 1},
            {"selfemployedlife", 1},
            {"aperolspritzswithmyfitzs", 1},
            {"fun", 2},
            {"family", 1},
            {"weekdayonpoint", 1},
            {"roadtomelbourne", 1},
            {"chanwontravel", 1},
            {"goodmorning", 1},
            {"celebrations", 1},
            {"bikecruising", 1},
            {"cycle", 1},
            {"suchatourist", 1},
            {"ootd", 1},
            {"drunk", 1},
            {"brightonpier", 1},
            {"dogbeach", 1},
            {"beachrd", 1},
            {"sandinourshoefordays", 1},
            {"cool", 1},
            {"fashion", 1},
            {"friends", 1},
            {"coffee", 2},
            {"structurefire", 1},
            {"hampton", 2},
            {"peterpan", 1},
            {"travel", 2},
            {"gobernies", 1},
            {"highcountry", 1},
            {"happynewyear", 1},
            {"lovemygiant", 1},
            {"goproaustralia", 1},
            {"sundays", 1},
            {"superbowlxlix", 1},
            {"cfmeu", 1},
            {"demviews", 1},
            {"work", 1},
            {"happiness", 1},
            {"selfie", 1},
            {"hugoboss", 1},
            {"happy", 3},
            {"sunnies", 1},
            {"roadtrip", 1},
            {"จะออกไปแตะขอบฟ้า", 1},
            {"ivote", 1},
            {"bringonsummer", 1},
            {"8harimencaricinta", 1},
            {"arajo", 1},
            {"sea", 1},
            {"whatdidiweartoday", 1},
            {"crapselfie", 1},
            {"weekend", 1},
            {"misikepekajaqkeostolia", 1},
            {"cputee", 1},
            {"cyclegroup", 1},
            {"walkingthedog", 3},
            {"nature", 1},
            {"challengemelb", 5},
            {"water", 1},
            {"jan1st", 1},
            {"yachtclub", 1},
            {"fitness", 1},
            {"repost", 1},
            {"hair", 1},
            {"runryan", 1},
            {"bunnings", 1}
        }
    },
    {
        145,
        {
            {"firealarm", 3},
            {"aflswansbombers", 1},
            {"whenwillitend", 1},
            {"unitethebeetsons", 1},
            {"longlost", 1},
            {"mufc", 1},
            {"hazmat", 1},
            {"bernieshithebeach", 1},
            {"mantra", 1},
            {"thatrainkissthough", 1},
            {"sissummer100", 1},
            {"incident", 2},
            {"sandyzebras", 1},
            {"chiropractic", 1},
            {"howsayingno", 1},
            {"merrychristmas", 1},
            {"strava", 1},
            {"structurefire", 1},
            {"hampton", 1},
            {"foodcoma", 1},
            {"lovegoodcoffee", 2},
            {"changemakers", 1},
            {"research", 1},
            {"arias", 1},
            {"friday", 1},
            {"retro", 1},
            {"avfc", 3},
            {"40s", 1},
            {"repost", 2},
            {"music", 1},
            {"granola", 1},
            {"famlife14", 2},
            {"febfast", 1},
            {"healthy", 1},
            {"jazz", 1},
            {"socent", 1},
            {"smoothies", 1},
            {"party", 1},
            {"sandringham", 2},
            {"amoshair", 1},
            {"warner", 1},
            {"linacreroad", 1},
            {"grandfinal", 1},
            {"berries", 1},
            {"tvd", 2},
            {"firstedition", 1},
            {"privatebeach", 1},
            {"entertainment", 1},
            {"singingtrio", 1},
            {"thankstony", 1},
            {"qanda", 1},
            {"vintage", 1},
            {"melbourne", 4},
            {"vafa", 1},
            {"yogurt", 1},
            {"superfoods", 1},
            {"footy", 1},
            {"blackrock", 1},
            {"lunch", 1},
            {"brotherneil", 1},
            {"vidalifestyle", 1},
            {"vb", 1},
            {"swimwithsharks", 1},
            {"health", 1},
            {"footscraybulldogs", 1},
            {"superstar", 1},
            {"endomondo", 1},
            {"melbweather", 1},
            {"happyfamily", 1},
            {"saganaki", 1},
            {"socialmedia", 1},
            {"marketing", 1},
            {"latergram", 1},
            {"bts15", 1},
            {"wishiwasakidagain", 1},
            {"livethedream", 1},
            {"realstrength", 1},
            {"hellenicrepublic", 1},
            {"osca", 1},
            {"diversity", 1},
            {"evaluation", 1},
            {"isitart", 1},
            {"sme", 1},
            {"bigdata", 1},
            {"endorphins", 1},
            {"tuesday", 1},
            {"sunset", 1},
            {"daylightsavings", 1},
            {"herewego", 1},
            {"sundaysesh", 1},
            {"empowering", 1},
            {"bully", 1},
            {"teamireland", 1},
            {"victraffic", 2}
        }
    },
    {0, {}},
    {0, {}},
    {
        378,
        {
            {"captainabbott", 1},
            {"entrepreneurship", 1},
            {"trust", 1},
            {"fitspo", 1},
            {"gopro", 1},
            {"willmissyou", 1},
            {"collection", 1},
            {"travelandhappiness", 1},
            {"viatgesacompanyats", 1},
            {"goldenpavilion", 1},
            {"sadface", 1},
            {"happy", 2},
            {"mygirl", 1},
            {"liketkit", 1},
            {"life", 1},
            {"bleak", 1},
            {"porridge", 1},
            {"cinnamon", 1},
            {"mate", 1},
            {"buildings", 1},
            {"люблюялето", 1},
            {"goodbyewinter", 1},
            {"ptlife", 1},
            {"early", 1},
            {"boxingday", 1},
            {"mustwatch", 1},
            {"oz", 2},
            {"nonstructure", 1},
            {"incident", 2},
            {"thatkindamorning", 1},
            {"socent", 1},
            {"rest", 1},
            {"tonkinesecats", 1},
            {"nye2015", 1},
            {"leadership", 1},
            {"women", 1},
            {"waste", 1},
            {"dontpanic", 1},
            {"winter", 3},
            {"hardlife", 1},
            {"workout", 1},
            {"drunk", 1},
            {"nofilter", 2},
            {"thoseeyesthough", 1},
            {"primalliving", 1},
            {"china", 1},
            {"exercise", 1},
            {"data", 1},
            {"analytics", 1},
            {"skyscraper", 1},
            {"nervous", 1},
            {"fitbuds", 1},
            {"fruit", 1},
            {"riseoftheidiots", 1},
            {"visitmelbourne", 1},
            {"beautiful", 1},
            {"homeday", 1},
            {"feelslikesummer", 1},
            {"iot", 1},
            {"burger", 1},
            {"estatemaster", 1},
            {"thebigv", 1},
            {"volunteer", 1},
            {"friends", 3},
            {"melbournesummer", 1},
            {"saturday", 2},
            {"strong", 2},
            {"lucky", 1},
            {"bullshit", 1},
            {"weekend", 2},
            {"plugga", 1},
            {"myboy", 3},
            {"sandringham", 13},
            {"fireplace", 1},
            {"icecreamvan", 1},
            {"bnw_demand", 1},
            {"training", 1},
            {"tourist", 2},
            {"neverstopexploring", 1},
            {"landscape", 1},
            {"sea", 2},
            {"myownfault", 1},
            {"pinhead", 1},
            {"thisface", 1},
            {"firealarm", 2},
            {"walkingthedog", 1},
            {"nature", 1},
            {"potd", 1},
            {"fyxo", 1},
            {"merrychristmas", 1},
            {"bay", 1},
            {"friyay", 1},
            {"snakes", 1},
            {"gazsenior", 1},
            {"weather", 1},
            {"justlovely", 1},
            {"riderfitcc", 1},
            {"kulanikinis", 1},
            {"fail", 1},
            {"freediving", 1},
            {"straya", 1},
            {"outsideisfree", 3},
            {"christmas", 1},
            {"happynewyear", 1},
            {"lovegoodexcercise", 1},
            {"sandrigham", 1},
            {"rocks", 2},
            {"sworks", 1},
            {"lookout", 1},
            {"sundaymorning", 1},
            {"melbourne", 24},
            {"laces", 1},
            {"saturdaynight", 1},
            {"architecture", 1},
            {"crackingviews", 1},
            {"bnw", 1},
            {"aussiebeach", 1},
            {"tabata", 1},
            {"qldbesties", 1},
            {"schoolholidays", 1},
            {"itsabitwarm", 1},
            {"noidea", 1},
            {"sunset", 5},
            {"brave", 1},
            {"ocean", 3},
            {"mosaics", 1},
            {"getinmybelly", 1},
            {"thatkindaday", 1},
            {"beach", 19},
            {"model", 1},
            {"bigdata", 1},
            {"blackandwhite", 1},
            {"eggallovermyface", 1},
            {"summer", 12},
            {"butfirstcoffee", 1},
            {"giraffe", 1},
            {"blackrock", 6},
            {"sub6", 1},
            {"bnw_planet", 1},
            {"nici", 2},
            {"breakfast", 2},
            {"burncity", 1},
            {"nosleep", 1},
            {"sickie", 1},
            {"cheeky", 1},
            {"thankgoodnessforyoga", 1},
            {"staffy", 1},
            {"selflovesunday", 1},
            {"volunteering", 1},
            {"fitness", 1},
            {"hookmeup", 1},
            {"brighton", 1},
            {"sand", 2},
            {"positive", 1},
            {"endomondo", 3},
            {"bayside", 2},
            {"nonstructurefire", 4},
            {"microsoft", 1},
            {"drinkprobs", 1},
            {"visitvictoria", 1},
            {"deepestdarkestasia", 1},
            {"victoriasecret", 1},
            {"spring", 4},
            {"halfmoonbay", 4},
            {"furbaby", 1},
            {"endorphins", 3},
            {"beachweather", 1},
            {"bnw_life_shots", 1},
            {"instagram", 1},
            {"beachboxes", 1},
            {"seashore", 1},
            {"admin", 1},
            {"rabbit", 1},
            {"zara", 1},
            {"firewatch", 1},
            {"friday", 1},
            {"cloud", 2},
            {"sandridgebeach", 1},
            {"axelhappy", 1},
            {"olympus", 1},
            {"wechat", 1},
            {"sky", 2},
            {"pavillion", 1},
            {"aussie", 1},
            {"australia", 9},
            {"shells", 1},
            {"victraffic", 7},
            {"sillouette", 1},
            {"movieanyone", 1},
            {"igers", 1},
            {"relationships", 1},
            {"palmtrees", 1},
            {"footy", 1},
            {"chocolate", 1},
            {"smartpeople", 1},
            {"christmastree", 1},
            {"love", 2},
            {"krismos", 1},
            {"imok", 1},
            {"moonbay", 1},
            {"happiness", 2},
            {"sweat", 1},
            {"family", 3},
            {"wagyu", 1},
            {"summerindecember", 1},
            {"scorcher", 1},
            {"sandy", 1},
            {"instadaily", 1},
            {"pebbles", 1},
            {"filter", 1},
            {"artisan", 1},
            {"cold", 1},
            {"sweep", 1},
            {"shootmenow", 1},
            {"sunsets", 1},
            {"nuts", 1},
            {"cliffs", 1},
            {"4shaw", 1},
            {"warm", 1},
            {"notfun", 1},
            {"views", 1},
            {"melbournelife", 2},
            {"fotosingh", 1},
            {"puffpastry", 1},
            {"crazyhound", 1},
            {"melborune", 2},
            {"ouch", 1},
            {"dayoff", 3},
            {"holidays", 2},
            {"sogrownup", 1},
            {"killer", 1},
            {"hehasmyheart", 1},
            {"bestoftheday", 1},
            {"healthy", 1},
            {"wellness", 1},
            {"afl", 1},
            {"sunny", 1},
            {"headache", 1},
            {"hehasmyeyes", 1},
            {"thingsthatshitjulia", 2},
            {"beachlife", 5},
            {"minid", 2},
            {"chillzone", 1},
            {"stilllovesacuddle", 1},
            {"lovehim", 1},
            {"ppb", 1},
            {"fire", 1},
            {"muscles", 1},
            {"cosy", 1},
            {"foodgloriousfood", 1},
            {"rickettspoint", 1},
            {"pizza", 1},
            {"caffeine", 1},
            {"food", 1},
            {"max", 1},
            {"swimwear", 1},
            {"beachlifestyle", 1},
            {"swimwearmodel", 1},
            {"run", 1},
            {"victoria", 3},
            {"stretch", 1},
            {"smile", 1},
            {"rockingrose", 1},
            {"ithink", 1},
            {"coffee", 2},
            {"structurefire", 2},
            {"sunshine", 2},
            {"besomeonessmile", 1},
            {"fridaynight", 1},
            {"crazyweather", 1},
            {"beauty", 1},
            {"stormy", 1},
            {"gotiges", 1},
            {"sandringhamyachtclub", 1},
            {"skytoday", 1},
            {"drinklocal", 1},
            {"supportlocal", 1},
            {"craftbeer", 1},
            {"indibeer", 1},
            {"tewinmelborune", 1},
            {"spearfishing", 1},
            {"christmasaustralia", 1},
            {"mrandmrsmoskov", 1},
            {"dontdoit", 1},
            {"sunday", 1},
            {"structure", 1},
            {"mosaic", 1},
            {"baysidebuyersagent", 1},
            {"mosaico", 1},
            {"travel", 2},
            {"mosaicart", 1},
            {"latergram", 1},
            {"propertydevelopment", 1},
            {"fit", 1},
            {"humpday", 1},
            {"furniture", 1},
            {"japan", 3},
            {"garden", 2},
            {"leatherjacket", 1},
            {"urbanangler", 1},
            {"damiankristof", 1},
            {"lovebicycle", 1},
            {"hot", 1},
            {"beachhut", 1},
            {"collectiveunitofnescafetraders", 1},
            {"jerkchicken", 1},
            {"mangosalsa", 1},
            {"sanset", 1},
            {"mkr", 3},
            {"thatsmelbourne", 1},
            {"hohum", 1},
            {"sailing", 1},
            {"govictory", 1},
            {"architexture", 1},
            {"celebchrissie", 1},
            {"fasting", 1},
            {"city", 1},
            {"urban", 1},
            {"socialgood", 1},
            {"travelnani", 1},
            {"vscocam", 1},
            {"sandringhambeach", 3},
            {"golden", 1},
            {"sundayfunday", 1},
            {"babes", 1},
            {"gorgeous", 1},
            {"temple", 1},
            {"wednesday", 1},
            {"clouds", 1},
            {"vibes", 1},
            {"xmas", 1},
            {"uncoolbutcool", 1},
            {"kinkuji", 2},
            {"sandybeach", 1},
            {"dillonmclg10", 1},
            {"tawan", 1},
            {"tropical", 1},
            {"nationalcoffeeday", 1},
            {"kyoto", 2},
            {"yummo", 1},
            {"foragecereal", 1},
            {"barney", 1},
            {"nfp", 1},
            {"warmday", 1},
            {"dashit", 1},
            {"mayneedtwotoday", 1},
            {"tourism", 1}
        }
    },
    {0, {}},
    {0, {}}
};

processor::result_type const processor_tester::process_tiny_ans {
    {
        {9, 472},
        {
            {"melbourne", 54},
            {"beach", 22},
            {"australia", 15},
            {"sunset", 15},
            {"brighton", 12},
            {"brightonbeach", 10},
            {"summer", 10}
        }
    },
    {
        {13, 378},
        {
            {"melbourne", 24},
            {"beach", 19},
            {"sandringham", 13},
            {"summer", 12},
            {"australia", 9}
        }
    },
    {
        {10, 145},
        {
            {"melbourne", 4},
            {"avfc", 3},
            {"firealarm", 3},
            {"famlife14", 2},
            {"incident", 2},
            {"lovegoodcoffee", 2},
            {"repost", 2},
            {"sandringham", 2},
            {"tvd", 2},
            {"victraffic", 2},
            {"40s", 1},
            {"aflswansbombers", 1},
            {"amoshair", 1},
            {"arias", 1},
            {"bernieshithebeach", 1},
            {"berries", 1},
            {"bigdata", 1},
            {"blackrock", 1},
            {"brotherneil", 1},
            {"bts15", 1},
            {"bully", 1},
            {"changemakers", 1},
            {"chiropractic", 1},
            {"daylightsavings", 1},
            {"diversity", 1},
            {"empowering", 1},
            {"endomondo", 1},
            {"endorphins", 1},
            {"entertainment", 1},
            {"evaluation", 1},
            {"febfast", 1},
            {"firstedition", 1},
            {"foodcoma", 1},
            {"footscraybulldogs", 1},
            {"footy", 1},
            {"friday", 1},
            {"grandfinal", 1},
            {"granola", 1},
            {"hampton", 1},
            {"happyfamily", 1},
            {"hazmat", 1},
            {"health", 1},
            {"healthy", 1},
            {"hellenicrepublic", 1},
            {"herewego", 1},
            {"howsayingno", 1},
            {"isitart", 1},
            {"jazz", 1},
            {"latergram", 1},
            {"linacreroad", 1},
            {"livethedream", 1},
            {"longlost", 1},
            {"lunch", 1},
            {"mantra", 1},
            {"marketing", 1},
            {"melbweather", 1},
            {"merrychristmas", 1},
            {"mufc", 1},
            {"music", 1},
            {"osca", 1},
            {"party", 1},
            {"privatebeach", 1},
            {"qanda", 1},
            {"realstrength", 1},
            {"research", 1},
            {"retro", 1},
            {"saganaki", 1},
            {"sandyzebras", 1},
            {"singingtrio", 1},
            {"sissummer100", 1},
            {"sme", 1},
            {"smoothies", 1},
            {"socent", 1},
            {"socialmedia", 1},
            {"strava", 1},
            {"structurefire", 1},
            {"sundaysesh", 1},
            {"sunset", 1},
            {"superfoods", 1},
            {"superstar", 1},
            {"swimwithsharks", 1},
            {"teamireland", 1},
            {"thankstony", 1},
            {"thatrainkissthough", 1},
            {"tuesday", 1},
            {"unitethebeetsons", 1},
            {"vafa", 1},
            {"vb", 1},
            {"vidalifestyle", 1},
            {"vintage", 1},
            {"warner", 1},
            {"whenwillitend", 1},
            {"wishiwasakidagain", 1},
            {"yogurt", 1}
        }
    },
    {{0, 0}, {}},
    {{1, 0}, {}},
    {{2, 0}, {}},
    {{3, 0}, {}},
    {{4, 0}, {}},
    {{5, 0}, {}},
    {{6, 0}, {}},
    {{7, 0}, {}},
    {{8, 0}, {}},
    {{11, 0}, {}},
    {{12, 0}, {}},
    {{14, 0}, {}},
    {{15, 0}, {}}
};
//@formatter:off

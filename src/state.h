#ifndef STATE_H
#define STATE_H

/* This is a generated file DO NOT EDIT, see state-vars.txt */

#include <gtk/gtk.h>

typedef void (*s_callback_func) (int id, float value);

typedef struct {
    int id;
    float value;
} s_entry;

void state_init();
void s_set_value_ui(int id, float value);
void s_set_value(int id, float value, int time);
void s_set_value_block(float *values, int base, int count);
void s_set_value_no_history(int id, float value);
void s_set_description(int id, const char *desc);
void s_clear_history();
void s_set_callback(int id, s_callback_func callback);
void s_set_adjustment(int id, GtkAdjustment * adjustment);
void s_history_add(const char *description);
void s_undo();
void s_redo();
void s_save_session_from_ui(GtkWidget * w, gpointer user_data);
void s_save_session(const char *fname);
void s_load_session_from_ui(GtkWidget * w, gpointer user_data);
void s_load_session(const char *fname);
void s_set_session_filename(const gchar * fname);
void s_crossfade(const int nframes);
void s_crossfade_ui();
void s_suppress_push();
void s_set_crossfade_time(float ct);
float s_get_crossfade_time();
void s_suppress_pop();
int s_have_session_filename();
gchar *s_get_session_filename();
void s_set_override_limiter_default();



#define S_NONE -1
#define S_LOAD 0
#define S_IN_GAIN 1
#define S_OUT_GAIN 2
#define S_IN_PAN 3
#define S_EQ_GAIN(n) (4 + n)
#define S_XOVER_FREQ(n) (1028 + n)
#define S_COMP_ATTACK(n) (1030 + n)
#define S_COMP_RELEASE(n) (1033 + n)
#define S_COMP_THRESH(n) (1036 + n)
#define S_COMP_RATIO(n) (1039 + n)
#define S_COMP_KNEE(n) (1042 + n)
#define S_COMP_MAKEUP(n) (1045 + n)
#define S_STEREO_WIDTH(n) (1048 + n)
#define S_LIM_INPUT 1051
#define S_LIM_TIME 1052
#define S_LIM_LIMIT 1053
#define S_BOOST 1054
#define S_LIM_LOGSCALE 1055
#define S_NOTCH_GAIN(n) (1056 + n)
#define S_NOTCH_FREQ(n) (1061 + n)
#define S_NOTCH_Q(n) (1066 + n)
#define S_NOTCH_FLAG(n) (1071 + n)
#define S_SIZE 1076

typedef struct {
    char *description;
    float value[S_SIZE];
} s_state;

void s_restore_state(s_state * state);
void s_crossfade_to_state(s_state * state, float time);
void s_history_add_state(s_state state);

extern float s_value[S_SIZE];

/* fetch currently used value */

inline static float s_get_value(int id)
{
    return s_value[id];
}

/* set value with no side effects */

inline static void s_set_value_ns(int id, float value)
{
    s_value[id] = value;
}

static const char *const s_description[S_SIZE] = {
    "Load file", "Input gain", "Output gain", "Input pan", "EQ Gain 1",
	"EQ Gain 2", "EQ Gain 3", "EQ Gain 4", "EQ Gain 5", "EQ Gain 6",
	"EQ Gain 7", "EQ Gain 8", "EQ Gain 9", "EQ Gain 10", "EQ Gain 11",
	"EQ Gain 12", "EQ Gain 13", "EQ Gain 14", "EQ Gain 15",
	"EQ Gain 16", "EQ Gain 17", "EQ Gain 18", "EQ Gain 19",
	"EQ Gain 20", "EQ Gain 21", "EQ Gain 22", "EQ Gain 23",
	"EQ Gain 24", "EQ Gain 25", "EQ Gain 26", "EQ Gain 27",
	"EQ Gain 28", "EQ Gain 29", "EQ Gain 30", "EQ Gain 31",
	"EQ Gain 32", "EQ Gain 33", "EQ Gain 34", "EQ Gain 35",
	"EQ Gain 36", "EQ Gain 37", "EQ Gain 38", "EQ Gain 39",
	"EQ Gain 40", "EQ Gain 41", "EQ Gain 42", "EQ Gain 43",
	"EQ Gain 44", "EQ Gain 45", "EQ Gain 46", "EQ Gain 47",
	"EQ Gain 48", "EQ Gain 49", "EQ Gain 50", "EQ Gain 51",
	"EQ Gain 52", "EQ Gain 53", "EQ Gain 54", "EQ Gain 55",
	"EQ Gain 56", "EQ Gain 57", "EQ Gain 58", "EQ Gain 59",
	"EQ Gain 60", "EQ Gain 61", "EQ Gain 62", "EQ Gain 63",
	"EQ Gain 64", "EQ Gain 65", "EQ Gain 66", "EQ Gain 67",
	"EQ Gain 68", "EQ Gain 69", "EQ Gain 70", "EQ Gain 71",
	"EQ Gain 72", "EQ Gain 73", "EQ Gain 74", "EQ Gain 75",
	"EQ Gain 76", "EQ Gain 77", "EQ Gain 78", "EQ Gain 79",
	"EQ Gain 80", "EQ Gain 81", "EQ Gain 82", "EQ Gain 83",
	"EQ Gain 84", "EQ Gain 85", "EQ Gain 86", "EQ Gain 87",
	"EQ Gain 88", "EQ Gain 89", "EQ Gain 90", "EQ Gain 91",
	"EQ Gain 92", "EQ Gain 93", "EQ Gain 94", "EQ Gain 95",
	"EQ Gain 96", "EQ Gain 97", "EQ Gain 98", "EQ Gain 99",
	"EQ Gain 100", "EQ Gain 101", "EQ Gain 102", "EQ Gain 103",
	"EQ Gain 104", "EQ Gain 105", "EQ Gain 106", "EQ Gain 107",
	"EQ Gain 108", "EQ Gain 109", "EQ Gain 110", "EQ Gain 111",
	"EQ Gain 112", "EQ Gain 113", "EQ Gain 114", "EQ Gain 115",
	"EQ Gain 116", "EQ Gain 117", "EQ Gain 118", "EQ Gain 119",
	"EQ Gain 120", "EQ Gain 121", "EQ Gain 122", "EQ Gain 123",
	"EQ Gain 124", "EQ Gain 125", "EQ Gain 126", "EQ Gain 127",
	"EQ Gain 128", "EQ Gain 129", "EQ Gain 130", "EQ Gain 131",
	"EQ Gain 132", "EQ Gain 133", "EQ Gain 134", "EQ Gain 135",
	"EQ Gain 136", "EQ Gain 137", "EQ Gain 138", "EQ Gain 139",
	"EQ Gain 140", "EQ Gain 141", "EQ Gain 142", "EQ Gain 143",
	"EQ Gain 144", "EQ Gain 145", "EQ Gain 146", "EQ Gain 147",
	"EQ Gain 148", "EQ Gain 149", "EQ Gain 150", "EQ Gain 151",
	"EQ Gain 152", "EQ Gain 153", "EQ Gain 154", "EQ Gain 155",
	"EQ Gain 156", "EQ Gain 157", "EQ Gain 158", "EQ Gain 159",
	"EQ Gain 160", "EQ Gain 161", "EQ Gain 162", "EQ Gain 163",
	"EQ Gain 164", "EQ Gain 165", "EQ Gain 166", "EQ Gain 167",
	"EQ Gain 168", "EQ Gain 169", "EQ Gain 170", "EQ Gain 171",
	"EQ Gain 172", "EQ Gain 173", "EQ Gain 174", "EQ Gain 175",
	"EQ Gain 176", "EQ Gain 177", "EQ Gain 178", "EQ Gain 179",
	"EQ Gain 180", "EQ Gain 181", "EQ Gain 182", "EQ Gain 183",
	"EQ Gain 184", "EQ Gain 185", "EQ Gain 186", "EQ Gain 187",
	"EQ Gain 188", "EQ Gain 189", "EQ Gain 190", "EQ Gain 191",
	"EQ Gain 192", "EQ Gain 193", "EQ Gain 194", "EQ Gain 195",
	"EQ Gain 196", "EQ Gain 197", "EQ Gain 198", "EQ Gain 199",
	"EQ Gain 200", "EQ Gain 201", "EQ Gain 202", "EQ Gain 203",
	"EQ Gain 204", "EQ Gain 205", "EQ Gain 206", "EQ Gain 207",
	"EQ Gain 208", "EQ Gain 209", "EQ Gain 210", "EQ Gain 211",
	"EQ Gain 212", "EQ Gain 213", "EQ Gain 214", "EQ Gain 215",
	"EQ Gain 216", "EQ Gain 217", "EQ Gain 218", "EQ Gain 219",
	"EQ Gain 220", "EQ Gain 221", "EQ Gain 222", "EQ Gain 223",
	"EQ Gain 224", "EQ Gain 225", "EQ Gain 226", "EQ Gain 227",
	"EQ Gain 228", "EQ Gain 229", "EQ Gain 230", "EQ Gain 231",
	"EQ Gain 232", "EQ Gain 233", "EQ Gain 234", "EQ Gain 235",
	"EQ Gain 236", "EQ Gain 237", "EQ Gain 238", "EQ Gain 239",
	"EQ Gain 240", "EQ Gain 241", "EQ Gain 242", "EQ Gain 243",
	"EQ Gain 244", "EQ Gain 245", "EQ Gain 246", "EQ Gain 247",
	"EQ Gain 248", "EQ Gain 249", "EQ Gain 250", "EQ Gain 251",
	"EQ Gain 252", "EQ Gain 253", "EQ Gain 254", "EQ Gain 255",
	"EQ Gain 256", "EQ Gain 257", "EQ Gain 258", "EQ Gain 259",
	"EQ Gain 260", "EQ Gain 261", "EQ Gain 262", "EQ Gain 263",
	"EQ Gain 264", "EQ Gain 265", "EQ Gain 266", "EQ Gain 267",
	"EQ Gain 268", "EQ Gain 269", "EQ Gain 270", "EQ Gain 271",
	"EQ Gain 272", "EQ Gain 273", "EQ Gain 274", "EQ Gain 275",
	"EQ Gain 276", "EQ Gain 277", "EQ Gain 278", "EQ Gain 279",
	"EQ Gain 280", "EQ Gain 281", "EQ Gain 282", "EQ Gain 283",
	"EQ Gain 284", "EQ Gain 285", "EQ Gain 286", "EQ Gain 287",
	"EQ Gain 288", "EQ Gain 289", "EQ Gain 290", "EQ Gain 291",
	"EQ Gain 292", "EQ Gain 293", "EQ Gain 294", "EQ Gain 295",
	"EQ Gain 296", "EQ Gain 297", "EQ Gain 298", "EQ Gain 299",
	"EQ Gain 300", "EQ Gain 301", "EQ Gain 302", "EQ Gain 303",
	"EQ Gain 304", "EQ Gain 305", "EQ Gain 306", "EQ Gain 307",
	"EQ Gain 308", "EQ Gain 309", "EQ Gain 310", "EQ Gain 311",
	"EQ Gain 312", "EQ Gain 313", "EQ Gain 314", "EQ Gain 315",
	"EQ Gain 316", "EQ Gain 317", "EQ Gain 318", "EQ Gain 319",
	"EQ Gain 320", "EQ Gain 321", "EQ Gain 322", "EQ Gain 323",
	"EQ Gain 324", "EQ Gain 325", "EQ Gain 326", "EQ Gain 327",
	"EQ Gain 328", "EQ Gain 329", "EQ Gain 330", "EQ Gain 331",
	"EQ Gain 332", "EQ Gain 333", "EQ Gain 334", "EQ Gain 335",
	"EQ Gain 336", "EQ Gain 337", "EQ Gain 338", "EQ Gain 339",
	"EQ Gain 340", "EQ Gain 341", "EQ Gain 342", "EQ Gain 343",
	"EQ Gain 344", "EQ Gain 345", "EQ Gain 346", "EQ Gain 347",
	"EQ Gain 348", "EQ Gain 349", "EQ Gain 350", "EQ Gain 351",
	"EQ Gain 352", "EQ Gain 353", "EQ Gain 354", "EQ Gain 355",
	"EQ Gain 356", "EQ Gain 357", "EQ Gain 358", "EQ Gain 359",
	"EQ Gain 360", "EQ Gain 361", "EQ Gain 362", "EQ Gain 363",
	"EQ Gain 364", "EQ Gain 365", "EQ Gain 366", "EQ Gain 367",
	"EQ Gain 368", "EQ Gain 369", "EQ Gain 370", "EQ Gain 371",
	"EQ Gain 372", "EQ Gain 373", "EQ Gain 374", "EQ Gain 375",
	"EQ Gain 376", "EQ Gain 377", "EQ Gain 378", "EQ Gain 379",
	"EQ Gain 380", "EQ Gain 381", "EQ Gain 382", "EQ Gain 383",
	"EQ Gain 384", "EQ Gain 385", "EQ Gain 386", "EQ Gain 387",
	"EQ Gain 388", "EQ Gain 389", "EQ Gain 390", "EQ Gain 391",
	"EQ Gain 392", "EQ Gain 393", "EQ Gain 394", "EQ Gain 395",
	"EQ Gain 396", "EQ Gain 397", "EQ Gain 398", "EQ Gain 399",
	"EQ Gain 400", "EQ Gain 401", "EQ Gain 402", "EQ Gain 403",
	"EQ Gain 404", "EQ Gain 405", "EQ Gain 406", "EQ Gain 407",
	"EQ Gain 408", "EQ Gain 409", "EQ Gain 410", "EQ Gain 411",
	"EQ Gain 412", "EQ Gain 413", "EQ Gain 414", "EQ Gain 415",
	"EQ Gain 416", "EQ Gain 417", "EQ Gain 418", "EQ Gain 419",
	"EQ Gain 420", "EQ Gain 421", "EQ Gain 422", "EQ Gain 423",
	"EQ Gain 424", "EQ Gain 425", "EQ Gain 426", "EQ Gain 427",
	"EQ Gain 428", "EQ Gain 429", "EQ Gain 430", "EQ Gain 431",
	"EQ Gain 432", "EQ Gain 433", "EQ Gain 434", "EQ Gain 435",
	"EQ Gain 436", "EQ Gain 437", "EQ Gain 438", "EQ Gain 439",
	"EQ Gain 440", "EQ Gain 441", "EQ Gain 442", "EQ Gain 443",
	"EQ Gain 444", "EQ Gain 445", "EQ Gain 446", "EQ Gain 447",
	"EQ Gain 448", "EQ Gain 449", "EQ Gain 450", "EQ Gain 451",
	"EQ Gain 452", "EQ Gain 453", "EQ Gain 454", "EQ Gain 455",
	"EQ Gain 456", "EQ Gain 457", "EQ Gain 458", "EQ Gain 459",
	"EQ Gain 460", "EQ Gain 461", "EQ Gain 462", "EQ Gain 463",
	"EQ Gain 464", "EQ Gain 465", "EQ Gain 466", "EQ Gain 467",
	"EQ Gain 468", "EQ Gain 469", "EQ Gain 470", "EQ Gain 471",
	"EQ Gain 472", "EQ Gain 473", "EQ Gain 474", "EQ Gain 475",
	"EQ Gain 476", "EQ Gain 477", "EQ Gain 478", "EQ Gain 479",
	"EQ Gain 480", "EQ Gain 481", "EQ Gain 482", "EQ Gain 483",
	"EQ Gain 484", "EQ Gain 485", "EQ Gain 486", "EQ Gain 487",
	"EQ Gain 488", "EQ Gain 489", "EQ Gain 490", "EQ Gain 491",
	"EQ Gain 492", "EQ Gain 493", "EQ Gain 494", "EQ Gain 495",
	"EQ Gain 496", "EQ Gain 497", "EQ Gain 498", "EQ Gain 499",
	"EQ Gain 500", "EQ Gain 501", "EQ Gain 502", "EQ Gain 503",
	"EQ Gain 504", "EQ Gain 505", "EQ Gain 506", "EQ Gain 507",
	"EQ Gain 508", "EQ Gain 509", "EQ Gain 510", "EQ Gain 511",
	"EQ Gain 512", "EQ Gain 513", "EQ Gain 514", "EQ Gain 515",
	"EQ Gain 516", "EQ Gain 517", "EQ Gain 518", "EQ Gain 519",
	"EQ Gain 520", "EQ Gain 521", "EQ Gain 522", "EQ Gain 523",
	"EQ Gain 524", "EQ Gain 525", "EQ Gain 526", "EQ Gain 527",
	"EQ Gain 528", "EQ Gain 529", "EQ Gain 530", "EQ Gain 531",
	"EQ Gain 532", "EQ Gain 533", "EQ Gain 534", "EQ Gain 535",
	"EQ Gain 536", "EQ Gain 537", "EQ Gain 538", "EQ Gain 539",
	"EQ Gain 540", "EQ Gain 541", "EQ Gain 542", "EQ Gain 543",
	"EQ Gain 544", "EQ Gain 545", "EQ Gain 546", "EQ Gain 547",
	"EQ Gain 548", "EQ Gain 549", "EQ Gain 550", "EQ Gain 551",
	"EQ Gain 552", "EQ Gain 553", "EQ Gain 554", "EQ Gain 555",
	"EQ Gain 556", "EQ Gain 557", "EQ Gain 558", "EQ Gain 559",
	"EQ Gain 560", "EQ Gain 561", "EQ Gain 562", "EQ Gain 563",
	"EQ Gain 564", "EQ Gain 565", "EQ Gain 566", "EQ Gain 567",
	"EQ Gain 568", "EQ Gain 569", "EQ Gain 570", "EQ Gain 571",
	"EQ Gain 572", "EQ Gain 573", "EQ Gain 574", "EQ Gain 575",
	"EQ Gain 576", "EQ Gain 577", "EQ Gain 578", "EQ Gain 579",
	"EQ Gain 580", "EQ Gain 581", "EQ Gain 582", "EQ Gain 583",
	"EQ Gain 584", "EQ Gain 585", "EQ Gain 586", "EQ Gain 587",
	"EQ Gain 588", "EQ Gain 589", "EQ Gain 590", "EQ Gain 591",
	"EQ Gain 592", "EQ Gain 593", "EQ Gain 594", "EQ Gain 595",
	"EQ Gain 596", "EQ Gain 597", "EQ Gain 598", "EQ Gain 599",
	"EQ Gain 600", "EQ Gain 601", "EQ Gain 602", "EQ Gain 603",
	"EQ Gain 604", "EQ Gain 605", "EQ Gain 606", "EQ Gain 607",
	"EQ Gain 608", "EQ Gain 609", "EQ Gain 610", "EQ Gain 611",
	"EQ Gain 612", "EQ Gain 613", "EQ Gain 614", "EQ Gain 615",
	"EQ Gain 616", "EQ Gain 617", "EQ Gain 618", "EQ Gain 619",
	"EQ Gain 620", "EQ Gain 621", "EQ Gain 622", "EQ Gain 623",
	"EQ Gain 624", "EQ Gain 625", "EQ Gain 626", "EQ Gain 627",
	"EQ Gain 628", "EQ Gain 629", "EQ Gain 630", "EQ Gain 631",
	"EQ Gain 632", "EQ Gain 633", "EQ Gain 634", "EQ Gain 635",
	"EQ Gain 636", "EQ Gain 637", "EQ Gain 638", "EQ Gain 639",
	"EQ Gain 640", "EQ Gain 641", "EQ Gain 642", "EQ Gain 643",
	"EQ Gain 644", "EQ Gain 645", "EQ Gain 646", "EQ Gain 647",
	"EQ Gain 648", "EQ Gain 649", "EQ Gain 650", "EQ Gain 651",
	"EQ Gain 652", "EQ Gain 653", "EQ Gain 654", "EQ Gain 655",
	"EQ Gain 656", "EQ Gain 657", "EQ Gain 658", "EQ Gain 659",
	"EQ Gain 660", "EQ Gain 661", "EQ Gain 662", "EQ Gain 663",
	"EQ Gain 664", "EQ Gain 665", "EQ Gain 666", "EQ Gain 667",
	"EQ Gain 668", "EQ Gain 669", "EQ Gain 670", "EQ Gain 671",
	"EQ Gain 672", "EQ Gain 673", "EQ Gain 674", "EQ Gain 675",
	"EQ Gain 676", "EQ Gain 677", "EQ Gain 678", "EQ Gain 679",
	"EQ Gain 680", "EQ Gain 681", "EQ Gain 682", "EQ Gain 683",
	"EQ Gain 684", "EQ Gain 685", "EQ Gain 686", "EQ Gain 687",
	"EQ Gain 688", "EQ Gain 689", "EQ Gain 690", "EQ Gain 691",
	"EQ Gain 692", "EQ Gain 693", "EQ Gain 694", "EQ Gain 695",
	"EQ Gain 696", "EQ Gain 697", "EQ Gain 698", "EQ Gain 699",
	"EQ Gain 700", "EQ Gain 701", "EQ Gain 702", "EQ Gain 703",
	"EQ Gain 704", "EQ Gain 705", "EQ Gain 706", "EQ Gain 707",
	"EQ Gain 708", "EQ Gain 709", "EQ Gain 710", "EQ Gain 711",
	"EQ Gain 712", "EQ Gain 713", "EQ Gain 714", "EQ Gain 715",
	"EQ Gain 716", "EQ Gain 717", "EQ Gain 718", "EQ Gain 719",
	"EQ Gain 720", "EQ Gain 721", "EQ Gain 722", "EQ Gain 723",
	"EQ Gain 724", "EQ Gain 725", "EQ Gain 726", "EQ Gain 727",
	"EQ Gain 728", "EQ Gain 729", "EQ Gain 730", "EQ Gain 731",
	"EQ Gain 732", "EQ Gain 733", "EQ Gain 734", "EQ Gain 735",
	"EQ Gain 736", "EQ Gain 737", "EQ Gain 738", "EQ Gain 739",
	"EQ Gain 740", "EQ Gain 741", "EQ Gain 742", "EQ Gain 743",
	"EQ Gain 744", "EQ Gain 745", "EQ Gain 746", "EQ Gain 747",
	"EQ Gain 748", "EQ Gain 749", "EQ Gain 750", "EQ Gain 751",
	"EQ Gain 752", "EQ Gain 753", "EQ Gain 754", "EQ Gain 755",
	"EQ Gain 756", "EQ Gain 757", "EQ Gain 758", "EQ Gain 759",
	"EQ Gain 760", "EQ Gain 761", "EQ Gain 762", "EQ Gain 763",
	"EQ Gain 764", "EQ Gain 765", "EQ Gain 766", "EQ Gain 767",
	"EQ Gain 768", "EQ Gain 769", "EQ Gain 770", "EQ Gain 771",
	"EQ Gain 772", "EQ Gain 773", "EQ Gain 774", "EQ Gain 775",
	"EQ Gain 776", "EQ Gain 777", "EQ Gain 778", "EQ Gain 779",
	"EQ Gain 780", "EQ Gain 781", "EQ Gain 782", "EQ Gain 783",
	"EQ Gain 784", "EQ Gain 785", "EQ Gain 786", "EQ Gain 787",
	"EQ Gain 788", "EQ Gain 789", "EQ Gain 790", "EQ Gain 791",
	"EQ Gain 792", "EQ Gain 793", "EQ Gain 794", "EQ Gain 795",
	"EQ Gain 796", "EQ Gain 797", "EQ Gain 798", "EQ Gain 799",
	"EQ Gain 800", "EQ Gain 801", "EQ Gain 802", "EQ Gain 803",
	"EQ Gain 804", "EQ Gain 805", "EQ Gain 806", "EQ Gain 807",
	"EQ Gain 808", "EQ Gain 809", "EQ Gain 810", "EQ Gain 811",
	"EQ Gain 812", "EQ Gain 813", "EQ Gain 814", "EQ Gain 815",
	"EQ Gain 816", "EQ Gain 817", "EQ Gain 818", "EQ Gain 819",
	"EQ Gain 820", "EQ Gain 821", "EQ Gain 822", "EQ Gain 823",
	"EQ Gain 824", "EQ Gain 825", "EQ Gain 826", "EQ Gain 827",
	"EQ Gain 828", "EQ Gain 829", "EQ Gain 830", "EQ Gain 831",
	"EQ Gain 832", "EQ Gain 833", "EQ Gain 834", "EQ Gain 835",
	"EQ Gain 836", "EQ Gain 837", "EQ Gain 838", "EQ Gain 839",
	"EQ Gain 840", "EQ Gain 841", "EQ Gain 842", "EQ Gain 843",
	"EQ Gain 844", "EQ Gain 845", "EQ Gain 846", "EQ Gain 847",
	"EQ Gain 848", "EQ Gain 849", "EQ Gain 850", "EQ Gain 851",
	"EQ Gain 852", "EQ Gain 853", "EQ Gain 854", "EQ Gain 855",
	"EQ Gain 856", "EQ Gain 857", "EQ Gain 858", "EQ Gain 859",
	"EQ Gain 860", "EQ Gain 861", "EQ Gain 862", "EQ Gain 863",
	"EQ Gain 864", "EQ Gain 865", "EQ Gain 866", "EQ Gain 867",
	"EQ Gain 868", "EQ Gain 869", "EQ Gain 870", "EQ Gain 871",
	"EQ Gain 872", "EQ Gain 873", "EQ Gain 874", "EQ Gain 875",
	"EQ Gain 876", "EQ Gain 877", "EQ Gain 878", "EQ Gain 879",
	"EQ Gain 880", "EQ Gain 881", "EQ Gain 882", "EQ Gain 883",
	"EQ Gain 884", "EQ Gain 885", "EQ Gain 886", "EQ Gain 887",
	"EQ Gain 888", "EQ Gain 889", "EQ Gain 890", "EQ Gain 891",
	"EQ Gain 892", "EQ Gain 893", "EQ Gain 894", "EQ Gain 895",
	"EQ Gain 896", "EQ Gain 897", "EQ Gain 898", "EQ Gain 899",
	"EQ Gain 900", "EQ Gain 901", "EQ Gain 902", "EQ Gain 903",
	"EQ Gain 904", "EQ Gain 905", "EQ Gain 906", "EQ Gain 907",
	"EQ Gain 908", "EQ Gain 909", "EQ Gain 910", "EQ Gain 911",
	"EQ Gain 912", "EQ Gain 913", "EQ Gain 914", "EQ Gain 915",
	"EQ Gain 916", "EQ Gain 917", "EQ Gain 918", "EQ Gain 919",
	"EQ Gain 920", "EQ Gain 921", "EQ Gain 922", "EQ Gain 923",
	"EQ Gain 924", "EQ Gain 925", "EQ Gain 926", "EQ Gain 927",
	"EQ Gain 928", "EQ Gain 929", "EQ Gain 930", "EQ Gain 931",
	"EQ Gain 932", "EQ Gain 933", "EQ Gain 934", "EQ Gain 935",
	"EQ Gain 936", "EQ Gain 937", "EQ Gain 938", "EQ Gain 939",
	"EQ Gain 940", "EQ Gain 941", "EQ Gain 942", "EQ Gain 943",
	"EQ Gain 944", "EQ Gain 945", "EQ Gain 946", "EQ Gain 947",
	"EQ Gain 948", "EQ Gain 949", "EQ Gain 950", "EQ Gain 951",
	"EQ Gain 952", "EQ Gain 953", "EQ Gain 954", "EQ Gain 955",
	"EQ Gain 956", "EQ Gain 957", "EQ Gain 958", "EQ Gain 959",
	"EQ Gain 960", "EQ Gain 961", "EQ Gain 962", "EQ Gain 963",
	"EQ Gain 964", "EQ Gain 965", "EQ Gain 966", "EQ Gain 967",
	"EQ Gain 968", "EQ Gain 969", "EQ Gain 970", "EQ Gain 971",
	"EQ Gain 972", "EQ Gain 973", "EQ Gain 974", "EQ Gain 975",
	"EQ Gain 976", "EQ Gain 977", "EQ Gain 978", "EQ Gain 979",
	"EQ Gain 980", "EQ Gain 981", "EQ Gain 982", "EQ Gain 983",
	"EQ Gain 984", "EQ Gain 985", "EQ Gain 986", "EQ Gain 987",
	"EQ Gain 988", "EQ Gain 989", "EQ Gain 990", "EQ Gain 991",
	"EQ Gain 992", "EQ Gain 993", "EQ Gain 994", "EQ Gain 995",
	"EQ Gain 996", "EQ Gain 997", "EQ Gain 998", "EQ Gain 999",
	"EQ Gain 1000", "EQ Gain 1001", "EQ Gain 1002", "EQ Gain 1003",
	"EQ Gain 1004", "EQ Gain 1005", "EQ Gain 1006", "EQ Gain 1007",
	"EQ Gain 1008", "EQ Gain 1009", "EQ Gain 1010", "EQ Gain 1011",
	"EQ Gain 1012", "EQ Gain 1013", "EQ Gain 1014", "EQ Gain 1015",
	"EQ Gain 1016", "EQ Gain 1017", "EQ Gain 1018", "EQ Gain 1019",
	"EQ Gain 1020", "EQ Gain 1021", "EQ Gain 1022", "EQ Gain 1023",
	"EQ Gain 1024", "Crossover frequency 1", "Crossover frequency 2",
	"Compressor attack 1", "Compressor attack 2",
	"Compressor attack 3", "Compressor release 1",
	"Compressor release 2", "Compressor release 3",
	"Compressor threshold 1", "Compressor threshold 2",
	"Compressor threshold 3", "Compressor ratio 1",
	"Compressor ratio 2", "Compressor ratio 3", "Compressor knee 1",
	"Compressor knee 2", "Compressor knee 3",
	"Compressor makeup gain 1", "Compressor makeup gain 2",
	"Compressor makeup gain 3", "Stereo width 1", "Stereo width 2",
	"Stereo width 3", "Limiter input gain", "Limiter time",
	"Limiter level", "Waveshaper boost", "Limiter logscale",
	"Notch gain 1", "Notch gain 2", "Notch gain 3", "Notch gain 4",
	"Notch gain 5", "Notch frequency 1", "Notch frequency 2",
	"Notch frequency 3", "Notch frequency 4", "Notch frequency 5",
	"Notch Q 1", "Notch Q 2", "Notch Q 3", "Notch Q 4", "Notch Q 5",
	"Notch active flag 1", "Notch active flag 2",
	"Notch active flag 3", "Notch active flag 4", "Notch active flag 5"
};

static const char *const s_symbol[S_SIZE] = {
    "load", "in-gain", "out-gain", "in-pan", "eq-gain0", "eq-gain1",
	"eq-gain2", "eq-gain3", "eq-gain4", "eq-gain5", "eq-gain6",
	"eq-gain7", "eq-gain8", "eq-gain9", "eq-gain10", "eq-gain11",
	"eq-gain12", "eq-gain13", "eq-gain14", "eq-gain15", "eq-gain16",
	"eq-gain17", "eq-gain18", "eq-gain19", "eq-gain20", "eq-gain21",
	"eq-gain22", "eq-gain23", "eq-gain24", "eq-gain25", "eq-gain26",
	"eq-gain27", "eq-gain28", "eq-gain29", "eq-gain30", "eq-gain31",
	"eq-gain32", "eq-gain33", "eq-gain34", "eq-gain35", "eq-gain36",
	"eq-gain37", "eq-gain38", "eq-gain39", "eq-gain40", "eq-gain41",
	"eq-gain42", "eq-gain43", "eq-gain44", "eq-gain45", "eq-gain46",
	"eq-gain47", "eq-gain48", "eq-gain49", "eq-gain50", "eq-gain51",
	"eq-gain52", "eq-gain53", "eq-gain54", "eq-gain55", "eq-gain56",
	"eq-gain57", "eq-gain58", "eq-gain59", "eq-gain60", "eq-gain61",
	"eq-gain62", "eq-gain63", "eq-gain64", "eq-gain65", "eq-gain66",
	"eq-gain67", "eq-gain68", "eq-gain69", "eq-gain70", "eq-gain71",
	"eq-gain72", "eq-gain73", "eq-gain74", "eq-gain75", "eq-gain76",
	"eq-gain77", "eq-gain78", "eq-gain79", "eq-gain80", "eq-gain81",
	"eq-gain82", "eq-gain83", "eq-gain84", "eq-gain85", "eq-gain86",
	"eq-gain87", "eq-gain88", "eq-gain89", "eq-gain90", "eq-gain91",
	"eq-gain92", "eq-gain93", "eq-gain94", "eq-gain95", "eq-gain96",
	"eq-gain97", "eq-gain98", "eq-gain99", "eq-gain100", "eq-gain101",
	"eq-gain102", "eq-gain103", "eq-gain104", "eq-gain105",
	"eq-gain106", "eq-gain107", "eq-gain108", "eq-gain109",
	"eq-gain110", "eq-gain111", "eq-gain112", "eq-gain113",
	"eq-gain114", "eq-gain115", "eq-gain116", "eq-gain117",
	"eq-gain118", "eq-gain119", "eq-gain120", "eq-gain121",
	"eq-gain122", "eq-gain123", "eq-gain124", "eq-gain125",
	"eq-gain126", "eq-gain127", "eq-gain128", "eq-gain129",
	"eq-gain130", "eq-gain131", "eq-gain132", "eq-gain133",
	"eq-gain134", "eq-gain135", "eq-gain136", "eq-gain137",
	"eq-gain138", "eq-gain139", "eq-gain140", "eq-gain141",
	"eq-gain142", "eq-gain143", "eq-gain144", "eq-gain145",
	"eq-gain146", "eq-gain147", "eq-gain148", "eq-gain149",
	"eq-gain150", "eq-gain151", "eq-gain152", "eq-gain153",
	"eq-gain154", "eq-gain155", "eq-gain156", "eq-gain157",
	"eq-gain158", "eq-gain159", "eq-gain160", "eq-gain161",
	"eq-gain162", "eq-gain163", "eq-gain164", "eq-gain165",
	"eq-gain166", "eq-gain167", "eq-gain168", "eq-gain169",
	"eq-gain170", "eq-gain171", "eq-gain172", "eq-gain173",
	"eq-gain174", "eq-gain175", "eq-gain176", "eq-gain177",
	"eq-gain178", "eq-gain179", "eq-gain180", "eq-gain181",
	"eq-gain182", "eq-gain183", "eq-gain184", "eq-gain185",
	"eq-gain186", "eq-gain187", "eq-gain188", "eq-gain189",
	"eq-gain190", "eq-gain191", "eq-gain192", "eq-gain193",
	"eq-gain194", "eq-gain195", "eq-gain196", "eq-gain197",
	"eq-gain198", "eq-gain199", "eq-gain200", "eq-gain201",
	"eq-gain202", "eq-gain203", "eq-gain204", "eq-gain205",
	"eq-gain206", "eq-gain207", "eq-gain208", "eq-gain209",
	"eq-gain210", "eq-gain211", "eq-gain212", "eq-gain213",
	"eq-gain214", "eq-gain215", "eq-gain216", "eq-gain217",
	"eq-gain218", "eq-gain219", "eq-gain220", "eq-gain221",
	"eq-gain222", "eq-gain223", "eq-gain224", "eq-gain225",
	"eq-gain226", "eq-gain227", "eq-gain228", "eq-gain229",
	"eq-gain230", "eq-gain231", "eq-gain232", "eq-gain233",
	"eq-gain234", "eq-gain235", "eq-gain236", "eq-gain237",
	"eq-gain238", "eq-gain239", "eq-gain240", "eq-gain241",
	"eq-gain242", "eq-gain243", "eq-gain244", "eq-gain245",
	"eq-gain246", "eq-gain247", "eq-gain248", "eq-gain249",
	"eq-gain250", "eq-gain251", "eq-gain252", "eq-gain253",
	"eq-gain254", "eq-gain255", "eq-gain256", "eq-gain257",
	"eq-gain258", "eq-gain259", "eq-gain260", "eq-gain261",
	"eq-gain262", "eq-gain263", "eq-gain264", "eq-gain265",
	"eq-gain266", "eq-gain267", "eq-gain268", "eq-gain269",
	"eq-gain270", "eq-gain271", "eq-gain272", "eq-gain273",
	"eq-gain274", "eq-gain275", "eq-gain276", "eq-gain277",
	"eq-gain278", "eq-gain279", "eq-gain280", "eq-gain281",
	"eq-gain282", "eq-gain283", "eq-gain284", "eq-gain285",
	"eq-gain286", "eq-gain287", "eq-gain288", "eq-gain289",
	"eq-gain290", "eq-gain291", "eq-gain292", "eq-gain293",
	"eq-gain294", "eq-gain295", "eq-gain296", "eq-gain297",
	"eq-gain298", "eq-gain299", "eq-gain300", "eq-gain301",
	"eq-gain302", "eq-gain303", "eq-gain304", "eq-gain305",
	"eq-gain306", "eq-gain307", "eq-gain308", "eq-gain309",
	"eq-gain310", "eq-gain311", "eq-gain312", "eq-gain313",
	"eq-gain314", "eq-gain315", "eq-gain316", "eq-gain317",
	"eq-gain318", "eq-gain319", "eq-gain320", "eq-gain321",
	"eq-gain322", "eq-gain323", "eq-gain324", "eq-gain325",
	"eq-gain326", "eq-gain327", "eq-gain328", "eq-gain329",
	"eq-gain330", "eq-gain331", "eq-gain332", "eq-gain333",
	"eq-gain334", "eq-gain335", "eq-gain336", "eq-gain337",
	"eq-gain338", "eq-gain339", "eq-gain340", "eq-gain341",
	"eq-gain342", "eq-gain343", "eq-gain344", "eq-gain345",
	"eq-gain346", "eq-gain347", "eq-gain348", "eq-gain349",
	"eq-gain350", "eq-gain351", "eq-gain352", "eq-gain353",
	"eq-gain354", "eq-gain355", "eq-gain356", "eq-gain357",
	"eq-gain358", "eq-gain359", "eq-gain360", "eq-gain361",
	"eq-gain362", "eq-gain363", "eq-gain364", "eq-gain365",
	"eq-gain366", "eq-gain367", "eq-gain368", "eq-gain369",
	"eq-gain370", "eq-gain371", "eq-gain372", "eq-gain373",
	"eq-gain374", "eq-gain375", "eq-gain376", "eq-gain377",
	"eq-gain378", "eq-gain379", "eq-gain380", "eq-gain381",
	"eq-gain382", "eq-gain383", "eq-gain384", "eq-gain385",
	"eq-gain386", "eq-gain387", "eq-gain388", "eq-gain389",
	"eq-gain390", "eq-gain391", "eq-gain392", "eq-gain393",
	"eq-gain394", "eq-gain395", "eq-gain396", "eq-gain397",
	"eq-gain398", "eq-gain399", "eq-gain400", "eq-gain401",
	"eq-gain402", "eq-gain403", "eq-gain404", "eq-gain405",
	"eq-gain406", "eq-gain407", "eq-gain408", "eq-gain409",
	"eq-gain410", "eq-gain411", "eq-gain412", "eq-gain413",
	"eq-gain414", "eq-gain415", "eq-gain416", "eq-gain417",
	"eq-gain418", "eq-gain419", "eq-gain420", "eq-gain421",
	"eq-gain422", "eq-gain423", "eq-gain424", "eq-gain425",
	"eq-gain426", "eq-gain427", "eq-gain428", "eq-gain429",
	"eq-gain430", "eq-gain431", "eq-gain432", "eq-gain433",
	"eq-gain434", "eq-gain435", "eq-gain436", "eq-gain437",
	"eq-gain438", "eq-gain439", "eq-gain440", "eq-gain441",
	"eq-gain442", "eq-gain443", "eq-gain444", "eq-gain445",
	"eq-gain446", "eq-gain447", "eq-gain448", "eq-gain449",
	"eq-gain450", "eq-gain451", "eq-gain452", "eq-gain453",
	"eq-gain454", "eq-gain455", "eq-gain456", "eq-gain457",
	"eq-gain458", "eq-gain459", "eq-gain460", "eq-gain461",
	"eq-gain462", "eq-gain463", "eq-gain464", "eq-gain465",
	"eq-gain466", "eq-gain467", "eq-gain468", "eq-gain469",
	"eq-gain470", "eq-gain471", "eq-gain472", "eq-gain473",
	"eq-gain474", "eq-gain475", "eq-gain476", "eq-gain477",
	"eq-gain478", "eq-gain479", "eq-gain480", "eq-gain481",
	"eq-gain482", "eq-gain483", "eq-gain484", "eq-gain485",
	"eq-gain486", "eq-gain487", "eq-gain488", "eq-gain489",
	"eq-gain490", "eq-gain491", "eq-gain492", "eq-gain493",
	"eq-gain494", "eq-gain495", "eq-gain496", "eq-gain497",
	"eq-gain498", "eq-gain499", "eq-gain500", "eq-gain501",
	"eq-gain502", "eq-gain503", "eq-gain504", "eq-gain505",
	"eq-gain506", "eq-gain507", "eq-gain508", "eq-gain509",
	"eq-gain510", "eq-gain511", "eq-gain512", "eq-gain513",
	"eq-gain514", "eq-gain515", "eq-gain516", "eq-gain517",
	"eq-gain518", "eq-gain519", "eq-gain520", "eq-gain521",
	"eq-gain522", "eq-gain523", "eq-gain524", "eq-gain525",
	"eq-gain526", "eq-gain527", "eq-gain528", "eq-gain529",
	"eq-gain530", "eq-gain531", "eq-gain532", "eq-gain533",
	"eq-gain534", "eq-gain535", "eq-gain536", "eq-gain537",
	"eq-gain538", "eq-gain539", "eq-gain540", "eq-gain541",
	"eq-gain542", "eq-gain543", "eq-gain544", "eq-gain545",
	"eq-gain546", "eq-gain547", "eq-gain548", "eq-gain549",
	"eq-gain550", "eq-gain551", "eq-gain552", "eq-gain553",
	"eq-gain554", "eq-gain555", "eq-gain556", "eq-gain557",
	"eq-gain558", "eq-gain559", "eq-gain560", "eq-gain561",
	"eq-gain562", "eq-gain563", "eq-gain564", "eq-gain565",
	"eq-gain566", "eq-gain567", "eq-gain568", "eq-gain569",
	"eq-gain570", "eq-gain571", "eq-gain572", "eq-gain573",
	"eq-gain574", "eq-gain575", "eq-gain576", "eq-gain577",
	"eq-gain578", "eq-gain579", "eq-gain580", "eq-gain581",
	"eq-gain582", "eq-gain583", "eq-gain584", "eq-gain585",
	"eq-gain586", "eq-gain587", "eq-gain588", "eq-gain589",
	"eq-gain590", "eq-gain591", "eq-gain592", "eq-gain593",
	"eq-gain594", "eq-gain595", "eq-gain596", "eq-gain597",
	"eq-gain598", "eq-gain599", "eq-gain600", "eq-gain601",
	"eq-gain602", "eq-gain603", "eq-gain604", "eq-gain605",
	"eq-gain606", "eq-gain607", "eq-gain608", "eq-gain609",
	"eq-gain610", "eq-gain611", "eq-gain612", "eq-gain613",
	"eq-gain614", "eq-gain615", "eq-gain616", "eq-gain617",
	"eq-gain618", "eq-gain619", "eq-gain620", "eq-gain621",
	"eq-gain622", "eq-gain623", "eq-gain624", "eq-gain625",
	"eq-gain626", "eq-gain627", "eq-gain628", "eq-gain629",
	"eq-gain630", "eq-gain631", "eq-gain632", "eq-gain633",
	"eq-gain634", "eq-gain635", "eq-gain636", "eq-gain637",
	"eq-gain638", "eq-gain639", "eq-gain640", "eq-gain641",
	"eq-gain642", "eq-gain643", "eq-gain644", "eq-gain645",
	"eq-gain646", "eq-gain647", "eq-gain648", "eq-gain649",
	"eq-gain650", "eq-gain651", "eq-gain652", "eq-gain653",
	"eq-gain654", "eq-gain655", "eq-gain656", "eq-gain657",
	"eq-gain658", "eq-gain659", "eq-gain660", "eq-gain661",
	"eq-gain662", "eq-gain663", "eq-gain664", "eq-gain665",
	"eq-gain666", "eq-gain667", "eq-gain668", "eq-gain669",
	"eq-gain670", "eq-gain671", "eq-gain672", "eq-gain673",
	"eq-gain674", "eq-gain675", "eq-gain676", "eq-gain677",
	"eq-gain678", "eq-gain679", "eq-gain680", "eq-gain681",
	"eq-gain682", "eq-gain683", "eq-gain684", "eq-gain685",
	"eq-gain686", "eq-gain687", "eq-gain688", "eq-gain689",
	"eq-gain690", "eq-gain691", "eq-gain692", "eq-gain693",
	"eq-gain694", "eq-gain695", "eq-gain696", "eq-gain697",
	"eq-gain698", "eq-gain699", "eq-gain700", "eq-gain701",
	"eq-gain702", "eq-gain703", "eq-gain704", "eq-gain705",
	"eq-gain706", "eq-gain707", "eq-gain708", "eq-gain709",
	"eq-gain710", "eq-gain711", "eq-gain712", "eq-gain713",
	"eq-gain714", "eq-gain715", "eq-gain716", "eq-gain717",
	"eq-gain718", "eq-gain719", "eq-gain720", "eq-gain721",
	"eq-gain722", "eq-gain723", "eq-gain724", "eq-gain725",
	"eq-gain726", "eq-gain727", "eq-gain728", "eq-gain729",
	"eq-gain730", "eq-gain731", "eq-gain732", "eq-gain733",
	"eq-gain734", "eq-gain735", "eq-gain736", "eq-gain737",
	"eq-gain738", "eq-gain739", "eq-gain740", "eq-gain741",
	"eq-gain742", "eq-gain743", "eq-gain744", "eq-gain745",
	"eq-gain746", "eq-gain747", "eq-gain748", "eq-gain749",
	"eq-gain750", "eq-gain751", "eq-gain752", "eq-gain753",
	"eq-gain754", "eq-gain755", "eq-gain756", "eq-gain757",
	"eq-gain758", "eq-gain759", "eq-gain760", "eq-gain761",
	"eq-gain762", "eq-gain763", "eq-gain764", "eq-gain765",
	"eq-gain766", "eq-gain767", "eq-gain768", "eq-gain769",
	"eq-gain770", "eq-gain771", "eq-gain772", "eq-gain773",
	"eq-gain774", "eq-gain775", "eq-gain776", "eq-gain777",
	"eq-gain778", "eq-gain779", "eq-gain780", "eq-gain781",
	"eq-gain782", "eq-gain783", "eq-gain784", "eq-gain785",
	"eq-gain786", "eq-gain787", "eq-gain788", "eq-gain789",
	"eq-gain790", "eq-gain791", "eq-gain792", "eq-gain793",
	"eq-gain794", "eq-gain795", "eq-gain796", "eq-gain797",
	"eq-gain798", "eq-gain799", "eq-gain800", "eq-gain801",
	"eq-gain802", "eq-gain803", "eq-gain804", "eq-gain805",
	"eq-gain806", "eq-gain807", "eq-gain808", "eq-gain809",
	"eq-gain810", "eq-gain811", "eq-gain812", "eq-gain813",
	"eq-gain814", "eq-gain815", "eq-gain816", "eq-gain817",
	"eq-gain818", "eq-gain819", "eq-gain820", "eq-gain821",
	"eq-gain822", "eq-gain823", "eq-gain824", "eq-gain825",
	"eq-gain826", "eq-gain827", "eq-gain828", "eq-gain829",
	"eq-gain830", "eq-gain831", "eq-gain832", "eq-gain833",
	"eq-gain834", "eq-gain835", "eq-gain836", "eq-gain837",
	"eq-gain838", "eq-gain839", "eq-gain840", "eq-gain841",
	"eq-gain842", "eq-gain843", "eq-gain844", "eq-gain845",
	"eq-gain846", "eq-gain847", "eq-gain848", "eq-gain849",
	"eq-gain850", "eq-gain851", "eq-gain852", "eq-gain853",
	"eq-gain854", "eq-gain855", "eq-gain856", "eq-gain857",
	"eq-gain858", "eq-gain859", "eq-gain860", "eq-gain861",
	"eq-gain862", "eq-gain863", "eq-gain864", "eq-gain865",
	"eq-gain866", "eq-gain867", "eq-gain868", "eq-gain869",
	"eq-gain870", "eq-gain871", "eq-gain872", "eq-gain873",
	"eq-gain874", "eq-gain875", "eq-gain876", "eq-gain877",
	"eq-gain878", "eq-gain879", "eq-gain880", "eq-gain881",
	"eq-gain882", "eq-gain883", "eq-gain884", "eq-gain885",
	"eq-gain886", "eq-gain887", "eq-gain888", "eq-gain889",
	"eq-gain890", "eq-gain891", "eq-gain892", "eq-gain893",
	"eq-gain894", "eq-gain895", "eq-gain896", "eq-gain897",
	"eq-gain898", "eq-gain899", "eq-gain900", "eq-gain901",
	"eq-gain902", "eq-gain903", "eq-gain904", "eq-gain905",
	"eq-gain906", "eq-gain907", "eq-gain908", "eq-gain909",
	"eq-gain910", "eq-gain911", "eq-gain912", "eq-gain913",
	"eq-gain914", "eq-gain915", "eq-gain916", "eq-gain917",
	"eq-gain918", "eq-gain919", "eq-gain920", "eq-gain921",
	"eq-gain922", "eq-gain923", "eq-gain924", "eq-gain925",
	"eq-gain926", "eq-gain927", "eq-gain928", "eq-gain929",
	"eq-gain930", "eq-gain931", "eq-gain932", "eq-gain933",
	"eq-gain934", "eq-gain935", "eq-gain936", "eq-gain937",
	"eq-gain938", "eq-gain939", "eq-gain940", "eq-gain941",
	"eq-gain942", "eq-gain943", "eq-gain944", "eq-gain945",
	"eq-gain946", "eq-gain947", "eq-gain948", "eq-gain949",
	"eq-gain950", "eq-gain951", "eq-gain952", "eq-gain953",
	"eq-gain954", "eq-gain955", "eq-gain956", "eq-gain957",
	"eq-gain958", "eq-gain959", "eq-gain960", "eq-gain961",
	"eq-gain962", "eq-gain963", "eq-gain964", "eq-gain965",
	"eq-gain966", "eq-gain967", "eq-gain968", "eq-gain969",
	"eq-gain970", "eq-gain971", "eq-gain972", "eq-gain973",
	"eq-gain974", "eq-gain975", "eq-gain976", "eq-gain977",
	"eq-gain978", "eq-gain979", "eq-gain980", "eq-gain981",
	"eq-gain982", "eq-gain983", "eq-gain984", "eq-gain985",
	"eq-gain986", "eq-gain987", "eq-gain988", "eq-gain989",
	"eq-gain990", "eq-gain991", "eq-gain992", "eq-gain993",
	"eq-gain994", "eq-gain995", "eq-gain996", "eq-gain997",
	"eq-gain998", "eq-gain999", "eq-gain1000", "eq-gain1001",
	"eq-gain1002", "eq-gain1003", "eq-gain1004", "eq-gain1005",
	"eq-gain1006", "eq-gain1007", "eq-gain1008", "eq-gain1009",
	"eq-gain1010", "eq-gain1011", "eq-gain1012", "eq-gain1013",
	"eq-gain1014", "eq-gain1015", "eq-gain1016", "eq-gain1017",
	"eq-gain1018", "eq-gain1019", "eq-gain1020", "eq-gain1021",
	"eq-gain1022", "eq-gain1023", "xover-freq0", "xover-freq1",
	"comp-attack0", "comp-attack1", "comp-attack2", "comp-release0",
	"comp-release1", "comp-release2", "comp-thresh0", "comp-thresh1",
	"comp-thresh2", "comp-ratio0", "comp-ratio1", "comp-ratio2",
	"comp-knee0", "comp-knee1", "comp-knee2", "comp-makeup0",
	"comp-makeup1", "comp-makeup2", "stereo-width0", "stereo-width1",
	"stereo-width2", "lim-input", "lim-time", "lim-limit", "boost",
	"lim-logscale", "notch-gain0", "notch-gain1", "notch-gain2",
	"notch-gain3", "notch-gain4", "notch-freq0", "notch-freq1",
	"notch-freq2", "notch-freq3", "notch-freq4", "notch-q0",
	"notch-q1", "notch-q2", "notch-q3", "notch-q4", "notch-flag0",
	"notch-flag1", "notch-flag2", "notch-flag3", "notch-flag4"
};

#endif

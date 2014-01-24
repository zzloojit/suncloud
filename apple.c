#include <glib.h>
#include <glib-object.h>
#include <stdio.h>



#define FRUIT_TYPE_APPLE		(fruit_apple_get_type())
#define FRUIT_APPLE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), FRUIT_TYPE_APPLE, FruitApple))
#define FRUIT_APLLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), FRUIT_TYPE_BAR, FruitAppleClass))
#define FRUIT_IS_APPLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), FRUIT_TYPE_APPLE))
#define FRUIT_IS_APPLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), FRUIT_TYPE_APPLE))
#define FRUIT_APPLE_GET_CLASSS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), FRUIT_TYPE_APPLE, FruitAppleClass))

typedef struct FruitApple		FruitApple;
typedef struct FruitAppleClass		FruitAppleClass;
typedef struct FruitApplePrivate	FruitApplePrivate;

#define FRUIT_APPLE_GET_RPIVATE(obj)					\
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), FRUIT_TYPE_APPLE, FruitApplePrivate))




struct FruitApplePrivate
{
  char* name;
  int weight;
};

struct FruitApple
{
  GObject parent;
  FruitApplePrivate* priv;
};

struct FruitAppleClass
{
  GObjectClass parent;
  /* signals */
  void (*apple_name)(FruitApple* fa, int n);
};

G_DEFINE_TYPE(FruitApple, fruit_apple, G_TYPE_OBJECT);

/* Properties */
enum {
  PROP_0,
  PROP_WEIGHT,
  PROP_NAME,
  PROP_N,
};

/* Signals */
enum {
  FRUIT_APPLE_NAME,
  FRUIT_APPLE_WEIGHT,
  FRUIT_APPLE_LAST_SIGNAL,
};

static guint signals[FRUIT_APPLE_LAST_SIGNAL];

static void fruit_apple_get_property(GObject	*gobject,
				     guint	 prop_id,
				     GValue	*value,
				     GParamSpec *pspec)
{
  FruitApple *fa = FRUIT_APPLE (gobject);
  FruitApplePrivate *fap = fa->priv;
  switch(prop_id) {
  case PROP_NAME:
    g_value_set_string(value, fap->name);
    break;
  case PROP_WEIGHT:
    g_value_set_int(value, fap->weight);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    break;
  }
}

static void fruit_apple_set_property(GObject		*gobject,
				     guint		 prop_id,
				     const GValue	*value,
				     GParamSpec		*pspec)
{
  FruitApple *fa = FRUIT_APPLE (gobject);
  FruitApplePrivate *fap = fa->priv;
  switch(prop_id) {
  case PROP_NAME:
    g_free(fap->name);
    fap->name = g_value_dup_string(value);
    g_signal_emit(gobject, signals[FRUIT_APPLE_NAME], 0, fap->name);
    break;
  case PROP_WEIGHT:
    fap->weight = g_value_get_int(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    break;
  }

}

static void fruit_apple_init(FruitApple* fa)
{

  fa->priv = FRUIT_APPLE_GET_RPIVATE(fa);
  printf("into %s\n", __FUNCTION__);
}

static void fruit_apple_class_init(FruitAppleClass* klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->set_property = fruit_apple_set_property;
  gobject_class->get_property = fruit_apple_get_property;

  g_object_class_install_property
    (gobject_class, PROP_WEIGHT,
     g_param_spec_int("apple-weight",
		      "apple weight",
		      "",
		      -1, INT_MAX, -1,
		      G_PARAM_READWRITE |
		      G_PARAM_STATIC_STRINGS));
    
  g_object_class_install_property
    (gobject_class, PROP_NAME,
     g_param_spec_string("apple-name",
			 "apple name",
			 "",
			 NULL,
			 G_PARAM_READWRITE |
			 G_PARAM_STATIC_STRINGS));

  signals[FRUIT_APPLE_NAME] = 
    g_signal_new("apple-name",
		 G_OBJECT_CLASS_TYPE(gobject_class),
		 G_SIGNAL_RUN_FIRST,
		 0,
		 NULL, NULL,
		 g_cclosure_marshal_VOID__STRING,
		 G_TYPE_NONE,
		 1,
		 G_TYPE_STRING);
  
  g_type_class_add_private(klass, sizeof(FruitApplePrivate));
  printf("into %s\n", __FUNCTION__);
}

void print_event(FruitApple* fa, gchar* name, gpointer data )
{
  printf("in function %s name = %s\n", __FUNCTION__, name);
  return;
}

int main(int argc, char **argv)
{
  g_type_init();
  FruitApple* fa = g_object_new (FRUIT_TYPE_APPLE, NULL);
  FruitApplePrivate* fap = FRUIT_APPLE_GET_RPIVATE(fa);
  printf("name = %s\nweight = %d\n", fap->name, fap->weight);
  g_signal_connect(fa,  "apple-name", print_event, NULL);

  g_object_set(fa, "apple-name", "banana", NULL);
  g_object_set(fa, "apple-weight", 200, NULL);

  int weight;
  const gchar *name;

  g_object_get(fa, "apple-name", &name, NULL);
  g_object_get(fa, "apple-weight", &weight, NULL);
  printf("name = %s\nweight = %d\n", name, weight);
  
  return 0;
}

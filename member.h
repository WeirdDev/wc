#ifndef MEMBER_H
#define MEMBER_H

/* THE PARENT CLASS OF ALL MEMBERS */
typedef int membertype;
#define TOKENS_TYPESTART		0x1000
#define GLOBALSPACE_TYPESTART	0x2000
#define LOCALSPACE_TYPESTART	0x3000
#define EXPRESSIONS_TYPESTART	0x4000

/* type number macros for quick definition, hiding the fact there are more type categories */
#define TKTN(number)	(TOKENS_TYPESTART+number)
#define GSTN(number)	(GLOBALSPACE_TYPESTART+number)
#define LSTN(number)	(LOCALSPACE_TYPESTART+number)
#define EXTN(number)	(EXPRESSIONS_TYPESTART+number)

/* conversion from any pointer type to pmember */
#define pmemconv(pointer)			((pmember)pointer)

/* is this member instance of this type? */
#define member_new(typename)		((p##typename )malloc(sizeof( typename )))
#define instanceof(member, type)	(member.type == type)

struct treevisitor_t;
struct member_t;
typedef void (*accept_method_ptr)(struct treevisitor_t* visitor, struct member_t* object);
struct member_t {
	membertype type;
	accept_method_ptr accept;
	int line;
};
typedef struct member_t member, *pmember;
extern void member_set(void* member, membertype type, accept_method_ptr accept, int line);

#endif

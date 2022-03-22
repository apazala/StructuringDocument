#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

struct word {
	char* data;
};

struct sentence {
	struct word* data;
	int word_count;//denotes number of words in a sentence
};

struct paragraph {
	struct sentence* data;
	int sentence_count;//denotes number of sentences in a paragraph
};

struct document {
	struct paragraph* data;
	int paragraph_count;//denotes number of paragraphs in a document
};

void add_paraph(struct document* d, struct paragraph* p) {
	d->data = (struct paragraph*) realloc(d->data, (d->paragraph_count + 1) * sizeof(struct paragraph));
	d->data[d->paragraph_count] = *p;
	d->paragraph_count++;
}

void add_word(struct sentence* sen, struct word* w)
{
	sen->data = (struct word*) realloc(sen->data, (sen->word_count + 1) * sizeof(struct word));
	sen->data[sen->word_count] = *w;
	sen->word_count++;
}

void add_sentence(struct paragraph* p, struct sentence* sen) {

	p->data = (struct sentence*) realloc(p->data, (p->sentence_count + 1) * sizeof(struct sentence));
	p->data[p->sentence_count] = *sen;
	p->sentence_count++;

}

void new_word(struct word* w, char *proc_word) {
	int cpylen = strlen(proc_word) + 1;

	w->data = (char *)malloc(cpylen * sizeof(char));
	strncpy(w->data, proc_word, cpylen);
}

struct document get_document(char* text) {

	char proccesed_word[MAX_CHARACTERS];
	struct document Doc = { NULL, 0 };
	struct word current_word = { NULL};
	struct sentence current_sentence = { NULL, 0 };
	struct paragraph current_paragraph = { NULL, 0 };


	int i = 0;
	char c;

	for (; (c = *text); text++) {
		if (c == ' ') {
			if (i != 0) { /*ignore empty words (ex: space after '.')*/
				proccesed_word[i] = '\0';
				new_word(&current_word, proccesed_word);
				add_word(&current_sentence, &current_word);
				
				i = 0;
			}
		}
		else if (c == '.') {

			proccesed_word[i] = '\0';
			new_word(&current_word, proccesed_word);
			add_word(&current_sentence, &current_word);

			i = 0;
			add_sentence(&current_paragraph, &current_sentence);
			/*sentence complete: reset for new sentence*/
			current_sentence.data = NULL;
			current_sentence.word_count = 0;
		}
		else if (c == '\n') {
			/*Word and sentence has already been added*/
			add_paraph(&Doc, &current_paragraph);

			/*paraph complete: reset for new paraph*/
			current_paragraph.data = NULL;
			current_paragraph.sentence_count = 0;
		}
		else {
			proccesed_word[i++] = c;
		}
	}


	/*Last word and sentence has already been added*/
	add_paraph(&Doc, &current_paragraph);

	return Doc;
}

struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n) {
	return Doc.data[n - 1].data[m - 1].data[k - 1];
}

struct sentence kth_sentence_in_mth_paragraph(struct document Doc, int k, int m) {
	return Doc.data[m - 1].data[k - 1];
}

struct paragraph kth_paragraph(struct document Doc, int k) {
	return Doc.data[k - 1];
}


void print_word(struct word w) {
	printf("%s", w.data);
}

void print_sentence(struct sentence sen) {
	for (int i = 0; i < sen.word_count; i++) {
		print_word(sen.data[i]);
		if (i != sen.word_count - 1) {
			printf(" ");
		}
	}
}

void print_paragraph(struct paragraph para) {
	for (int i = 0; i < para.sentence_count; i++) {
		print_sentence(para.data[i]);
		printf(".");
	}
}

void print_document(struct document doc) {
	for (int i = 0; i < doc.paragraph_count; i++) {
		print_paragraph(doc.data[i]);
		if (i != doc.paragraph_count - 1)
			printf("\n");
	}
}

char* get_input_text() {
	int paragraph_count;
	scanf("%d", &paragraph_count);

	char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
	memset(doc, 0, sizeof(doc));
	getchar();
	for (int i = 0; i < paragraph_count; i++) {
		scanf("%[^\n]%*c", p[i]);
		strcat(doc, p[i]);
		if (i != paragraph_count - 1)
			strcat(doc, "\n");
	}

	char* returnDoc = (char*)malloc((strlen(doc) + 1) * (sizeof(char)));
	strcpy(returnDoc, doc);
	return returnDoc;
}

int main()
{
	char* text = get_input_text();
	struct document Doc = get_document(text);

	int q;
	scanf("%d", &q);

	while (q--) {
		int type;
		scanf("%d", &type);

		if (type == 3) {
			int k, m, n;
			scanf("%d %d %d", &k, &m, &n);
			struct word w = kth_word_in_mth_sentence_of_nth_paragraph(Doc, k, m, n);
			print_word(w);
		}

		else if (type == 2) {
			int k, m;
			scanf("%d %d", &k, &m);
			struct sentence sen = kth_sentence_in_mth_paragraph(Doc, k, m);
			print_sentence(sen);
		}

		else {
			int k;
			scanf("%d", &k);
			struct paragraph para = kth_paragraph(Doc, k);
			print_paragraph(para);
		}
		printf("\n");
	}
}
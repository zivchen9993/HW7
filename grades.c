#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked-list.h"
#include "grades.h"

// TYPEDEF:
typedef struct list *plist_t;
typedef struct node *pnode_t;

// FUNCTIONS DECLARATIONS:
int courses_clone(void *element, void **output);
void courses_destroy(void *element);
int student_clone(void *element, void **output);
void student_destroy(void *element);
struct student *list_search_id(struct grades *grades, int check_id);
int course_exist(struct student *student, const char *check_name);
int print_courses(struct student *print_student_course);

// STRUCTS:
struct student {
  int id;
  char *student_name;
  plist_t courses_list;
};

struct grades {
  plist_t students;
};

struct courses {
  int grade;
  char *course_name;
};

//FUNCTIONS IMPLEMENTATIONS:
/**
 * user function for the ADT.
 * @param element: the course we want to clone.
 * @param output: the pointer to the cloned course will be in the "output"
 * address.
 * @return: 0 on success (cloning process), 1 otherwise.
 */
int courses_clone(void *element, void **output) {
  if (element == NULL || output == NULL) {
    return 1;
  }
  struct courses *old_courses = (struct courses *) element;
  struct courses
      *new_courses = (struct courses *) malloc(sizeof(struct courses));
  if (new_courses == NULL) {
    return 1;
  }
  new_courses->grade = old_courses->grade;
  new_courses->course_name =
      (char *) malloc(strlen(old_courses->course_name) + 1);
  if (new_courses->course_name == NULL) {
    free(new_courses);
    return 1;
  }
  strcpy(new_courses->course_name, old_courses->course_name);
  *output = (void *) new_courses;
  return 0;
}

/**
 * user function for the ADT.
 * free's the course name and the course itself.
 * @param element: the course we want to free.
 */
void courses_destroy(void *element) {
  if (element == NULL) {
    return;
  }
  struct courses *courses = (struct courses *) element;
  free(courses->course_name);
  free(courses);
}

/**
 * user function for the ADT.
 * @param element: the student we want to clone.
 * @param output: the pointer to the cloned student will be in the "output"
 * address.
 * @return: 0 on success (cloning process), 1 otherwise.
 */
int student_clone(void *element, void **output) {
  if (element == NULL || output == NULL) {
    return 1;
  }
  struct student *old_student = (struct student *) element;
  struct student
      *new_student = (struct student *) malloc(sizeof(struct student));
  if (new_student == NULL) {
    return 1;
  }
  new_student->id = old_student->id;
  new_student->student_name =
      (char *) malloc(strlen(old_student->student_name) + 1);
  if (new_student->student_name == NULL) {
    free(new_student);
    return 1;
  }
  strcpy(new_student->student_name, old_student->student_name);
  new_student->courses_list = list_init(courses_clone, courses_destroy);
  if (new_student->courses_list == NULL) {
    free(new_student->student_name);
    free(new_student);
    return 1;
  }
  pnode_t iterator = list_begin(old_student->courses_list);
  while (iterator != NULL) {
    pnode_t it_last_grade_new_student = list_end(new_student->courses_list);
    void *get_iterator = list_get(iterator);
    if (get_iterator == NULL) {
      free(new_student->student_name);
      free(new_student);
      return 1;
    }
    int course_list_success = list_insert(new_student->courses_list,
                        it_last_grade_new_student,
                        get_iterator);
    if (course_list_success != 0) {
      free(new_student->student_name);
      free(new_student);
      return 1;
    }
    iterator = list_next(iterator);
  }
  *output = (void *) new_student;
  return 0;
}

/**
 * user function for the ADT.
 * free's the student name, student's courses list and the student itself.
 * @param element: the student we want to free.
 */
void student_destroy(void *element) {
  if (element == NULL) {
    return;
  }
  struct student *student = (struct student *) element;
  free(student->student_name);
  list_destroy(student->courses_list);
  free(student);
}

/**
 * search the student with the given ID in the students list.
 * @param grades: the list we want to search in.
 * @param check_id: the ID of the student we want to find.
 * @return pointer to the student we've found. return NULL if the student
 * isn't on the list or on error
 */
struct student *list_search_id(struct grades *grades, int check_id) {
  if (grades == NULL) {
    return NULL;
  }
  pnode_t iterator = list_begin(grades->students);
  if (iterator == NULL) {
    return NULL;
  }
  while (iterator != NULL) {
    struct student *current_student = (struct student *) list_get(iterator);
    if (current_student == NULL) {
      return NULL;
    }
    int current_id = current_student->id;
    if (check_id == current_id) {
      return current_student;
    }
    iterator = list_next(iterator);
  }
  return NULL;
}

/**
 * check's if the course is already on a given student's course list.
 * @param student: the student we want to check his courses.
 * @param check_name: the course name we want to check.
 * @return: 0 when the given course is already on the courses list, 1 otherwise.
 */
int course_exist(struct student *student, const char *check_name) {
  if ((student == NULL) || check_name == NULL) {
    return 1;
  }
  pnode_t iterator = list_begin(student->courses_list);
  while (iterator != NULL) {
    struct courses *current_course = (struct courses *) list_get(iterator);
    if (current_course == NULL) {
      return 1;
    }
    char *current_course_name = current_course->course_name;
    if (strcmp(current_course_name, check_name) == 0) {
      return 0;
    }
    iterator = list_next(iterator);
  }
  return 1;
}

/**
 * prints all the courses of a given student.
 * @param print_student_course: the student which we want to print his courses.
 * @return: 0 on success, 1 otherwise.
 */
int print_courses(struct student *print_student_course) {
  if (print_student_course == NULL) {
    return 1;
  }
  pnode_t iterator = list_begin(print_student_course->courses_list);
  struct courses *course_to_print;
  while (iterator != NULL) {
    course_to_print = (struct courses *) list_get(iterator);
    if (course_to_print == NULL) {
      return 1;
    }
    iterator = list_next(iterator);
    if (iterator == NULL) {
      printf(" %s %d", course_to_print->course_name, course_to_print->grade);
    } else {
      printf(" %s %d,", course_to_print->course_name, course_to_print->grade);
    }
  }
  printf("\n");
  return 0;
}

////////////////////////////////////////////////
/////////////////grades.h functions/////////////
////////////////////////////////////////////////

struct grades *grades_init() {
  struct grades *new_grades = (struct grades *) malloc(sizeof(struct grades));
  if (new_grades == NULL) {
    return NULL;
  }
  new_grades->students = list_init(student_clone, student_destroy);
  if (new_grades->students == NULL) {
    return NULL;
  }
  return new_grades;
}

void grades_destroy(struct grades *grades) {
  if (grades == NULL) {
    return;
  }
  list_destroy(grades->students);
  free(grades);
}

int grades_add_student(struct grades *grades, const char *name, int id) {
  if ((grades == NULL) || (name == NULL)) {
    return 1;
  }
  if (list_search_id(grades, id) != NULL) {
    return 1;
  }
  struct student new_student;
  new_student.id = id;
  new_student.student_name = (char *) malloc(strlen(name) + 1);
  if (new_student.student_name == NULL) {
    return 1;
  }
  strcpy(new_student.student_name, name);
  new_student.courses_list = list_init(courses_clone, courses_destroy);
  if (new_student.courses_list == NULL) {
    free(new_student.student_name);
    return 1;
  }
  void *void_new_student = (void *) (&new_student);
  list_push_back(grades->students, void_new_student);
  free(new_student.student_name);
  list_destroy(new_student.courses_list);
  return 0;
}

int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade) {
  if (grade < 0 || grade > 100) {
    return 1;
  } else if ((grades == NULL) || (name == NULL)) {
    return 1;
  }else if (grades->students == NULL) {
    return 1;
  }
  struct student *student_to_add_grade = list_search_id(grades, id);
  if (student_to_add_grade == NULL) {
    return 1;
  } else if (course_exist(student_to_add_grade, name) == 0) {
    return 1;
  }
  struct courses new_course;
  new_course.grade = grade;
  new_course.course_name = (char *) malloc(strlen(name) + 1);
  if (new_course.course_name == NULL) {
    return 1;
  }
  strcpy(new_course.course_name, name);
  void *void_new_course = (void *) (&new_course);
  if (list_push_back(student_to_add_grade->courses_list, void_new_course)
      != 0) {
    free(new_course.course_name);
    return 1;
  }
  free(new_course.course_name);
  return 0;
}

float grades_calc_avg(struct grades *grades, int id, char **out) {
  if ((grades == NULL) || (out == NULL)) {
    return -1;
  }
  struct student *student_to_avg = list_search_id(grades, id);
  if (student_to_avg == NULL) {
    *out = NULL;
    return -1;
  }
  char *ps_name = (char *) malloc(strlen(student_to_avg->student_name) + 1);
  if (ps_name == NULL) {
    *out = NULL;
    return -1;
  }
  strcpy(ps_name, student_to_avg->student_name);
  *out = ps_name;
  pnode_t avg_courses = list_begin(student_to_avg->courses_list);
  if (avg_courses == NULL) {
    return 0;
  }
  int grade_sum = 0;
  int courses_amount = 0;
  pnode_t iterator = list_begin(student_to_avg->courses_list);
  while (iterator != NULL) {
    struct courses *current_course = (struct courses *) list_get(iterator);
    if (current_course == NULL) {
      *out = NULL;
      free(ps_name);
      return -1;
    }
    grade_sum += current_course->grade;
    courses_amount++;
    iterator = list_next(iterator);
  }
  float avg;
  avg = ((float) grade_sum / (float) courses_amount);
  return avg;
}

int grades_print_student(struct grades *grades, int id) {
  if (grades == NULL) {
    return 1;
  }
  else if (grades->students == NULL) {
    return 1;
  }
  struct student *student_to_print = (list_search_id(grades, id));
  if (student_to_print == NULL) {
    return 1;
  }
  printf("%s %d:", student_to_print->student_name, id);
  int print_success = print_courses(student_to_print);
  if (print_success != 0) {
    return 1;
  } else {
    return 0;
  }
}

int grades_print_all(struct grades *grades) {
  if (grades == NULL) {
    return 1;
  }
  pnode_t iterator = list_begin(grades->students);
  while (iterator != NULL) {
    struct student *current_student = (struct student *) list_get(iterator);
    if (current_student == NULL) {
      return 1;
    }
    printf("%s %d:", current_student->student_name, current_student->id);
    iterator = list_next(iterator);
    int print_success = print_courses(current_student);
    if (print_success != 0) {
      return 1;
    }
  }
  return 0;
}
# CLRS_homework

## Several Problems
### Assignment 1
- One should use `std::span` for all sort method argument, instead of a `std::vector` and a strange wrapper (`std::span` hasn't been implemented when I write this, about 2020 autumn)
- OpenMP in this assigntment is not reliable, one should use task clause to rewrite it (I didn't have too much time on elevating program to multi-threading program)
- Use `std::source_location` (hasn't been implemented in clang, 2021 spring) to replace some strange macro

### Assignment 2
- OpenMP in this assigntment is not reliable, one should use task clause to rewrite it (I didn't have too much time on elevating program to multi-threading program)

### Assignment 3
- No problem, extensive use of template to simplify symmetry operation makes program hard to read

### Assignmet 4
- No problem, OpenMP works great here 

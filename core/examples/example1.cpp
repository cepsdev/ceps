





TEST(
R"ceps( 
 
assert{
 negate(negate(1)) == 1;
 negate(-2) == 2;
};

)ceps"
)

int negate(int j) {return -j;}


int main(){
  ceps::reg(negate,"negate");
  RUN_ALL_TESTS;  
}
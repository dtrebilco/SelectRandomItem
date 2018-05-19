extern crate rand;

use rand::Rng;
use rand::distributions::Uniform;

// Generic trait on an iterator
trait TakeRand<I> where I: Sized{
    fn take_rand<R: Rng + ?Sized>(self, rand : &mut R) -> Option<I>;
}

impl<I:Iterator> TakeRand<I::Item> for I {
    fn take_rand<R: Rng + ?Sized>(self, rand : &mut R) -> Option<I::Item> {
        let mut count : usize = 0;
        let mut ret_val = None;
        for i in self {
            // Check if selected
            count += 1;
            if Uniform::sample_single(0, count, rand) == 0 {
                ret_val = Some(i);
            }
        }
        ret_val
    }
}

// Single function
pub fn sample_one_iter<T, I, R>(rng: &mut R, iterable: I) -> Option<T>
    where I: IntoIterator<Item=T>,
          R: Rng + ?Sized,
{
    let mut count : usize = 0;
    let mut ret_val = None;
    for i in iterable {
        // Check if selected
        count += 1;
        if Uniform::sample_single(0, count, rng) == 0 {
            ret_val = Some(i);
        }
    }
    ret_val
}

fn main() {

    let mut meals: Vec<&str> = vec!["breakfast", "lunch", "dinner"];
    meals.push("str");

    let mut rng = rand::thread_rng();

    //let val :Option<&str> = sample_one_iter(&mut rng, meals);

    // Take a random item that matches a filter
    let val =  meals.iter().filter(|x| x.find('a') != None).take_rand(&mut rng);
    match val {
        Some(x) => println!("Found {}", x),
        None => println!("Not found")
    }
    
}    

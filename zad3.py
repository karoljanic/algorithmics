# longest sufix - prefix

# Function to find the longest prefix of str1 that is also a suffix of str2
# Returns the length of the longest prefix that is also a suffix
def longest_sufix_prefix(str1: str, str2: str) -> int:
    for k in range(min(len(str1), len(str2)), 0, -1):
        if str1[:k] == str2[-k:]:
            return k
    return 0


# Read str1 and str2 from input and print the result
str1 = input("Enter str1: ")
str2 = input("Enter str2: ")
print(f"The longest prefix of str1 that is also a suffix of str2 has length: {longest_sufix_prefix(str1, str2)}")

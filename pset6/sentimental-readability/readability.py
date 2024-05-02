from cs50 import get_string


def main():
    # Get text
    text = get_string("Text: ")
    # Get number of letters, words, and sentences
    numLetters = count_letters(text)
    numWords = count_words(text)
    numSentences = count_sentences(text)
    # Computer L (number of letters per 100 words) and S (number of sentences per 100 words)
    L = (numLetters / numWords) * 100
    S = (numSentences / numWords) * 100
    # Compute the index using L and S
    index = 0.0588 * L - 0.296 * S - 15.8
    # Round the index to get the grade
    grade = round(index)
    # Print the grade
    if grade >= 1 and grade < 16:
        print("Grade " + str(grade))
    elif grade >= 16:
        print("Grade 16+")
    else:
        print("Before Grade 1")


def count_letters(text):
    # Intantiate letter count
    letterCount = 0
    # Iterate over each character
    for c in text:
        # If a character is a letter, add to the letter count
        if c.isalpha():
            letterCount += 1
    return letterCount


def count_words(text):
    # Intantiate word count
    wordCount = 0
    # Iterate over each character
    for c in text:
        # If there is a space, we know the characters before it were a word
        if c == ' ':
            wordCount += 1
    # Add one in the end to account for the last word - no space after
    return wordCount + 1


def count_sentences(text):
    # Intantiate sentence count
    sentenceCount = 0
    # Iterate over each character
    for c in text:
        # If there is a . ! ? then it is ending a sentence, so we add to sentecne counter
        if c == '.' or c == '!' or c == '?':
            sentenceCount += 1
    return sentenceCount


if __name__ == "__main__":
    main()

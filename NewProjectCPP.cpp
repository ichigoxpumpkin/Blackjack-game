#include <iostream>
#include <array>
#include <algorithm>
#include <ctime>
#include <random>
#include <cassert>
#include <numeric>
#include <vector>

enum class CardRank
{
	rank_2,
	rank_3,
	rank_4,
	rank_5,
	rank_6,
	rank_7,
	rank_8,
	rank_9,
	rank_10,
	rank_jack,
	rank_queen,
	rank_king,
	rank_ace,

	maxRanks,
};

enum class CardSuit
{
	suit_clubs,
	suit_diamonds,
	suit_hearts,
	suit_spades,

	maxSuits,
};

using enum CardRank;
using enum CardSuit;

struct Card
{
	CardRank rank{};
	CardSuit suit{};
};

void printCard(const Card& card)
{

	switch (card.rank)
	{
	case rank_2:		std::cout << '2';	break;
	case rank_3:		std::cout << '3';	break;
	case rank_4:		std::cout << '4';	break;
	case rank_5:		std::cout << '5';	break;
	case rank_6:		std::cout << '6';	break;
	case rank_7:		std::cout << '7';	break;
	case rank_8:		std::cout << '8';	break;
	case rank_9:		std::cout << '9';	break;
	case rank_10:		std::cout << 'T';	break;
	case rank_king:		std::cout << 'K';	break;
	case rank_ace:		std::cout << 'A';	break;
	case rank_jack:		std::cout << 'J';	break;
	case rank_queen:	std::cout << 'Q';	break;
	default:			std::cout << '?';
	}

	switch (card.suit)
	{
	case suit_clubs:	std::cout << 'C';	break;
	case suit_diamonds:	std::cout << 'D';	break;
	case suit_hearts:	std::cout << 'H';	break;
	case suit_spades:	std::cout << 'S';	break;
	default:			std::cout << '?';
	}
	std::cout << ' ';
}
using Deck = std::array<Card, 52>;

Deck createDeck()
{
	Deck create_deck;

	for (int i{ 0 }; i < static_cast<int>(maxSuits); ++i)
	{
		for (int k{ 0 }; k < static_cast<int>(maxRanks); ++k)
		{
			create_deck[k + i * static_cast<int>(maxRanks)] = { static_cast<CardRank>(k), static_cast<CardSuit>(i) };
		}
	}

	return create_deck;
}

void printDeck(const Deck& deck)
{
	for (const auto& card : deck)
	{
		printCard(card);
	}
	std::cout << '\n';
}

int getCardValue(const Card& card)
{
	switch (card.rank)
	{
	case rank_2: return 2;
	case rank_3: return 3;
	case rank_4: return 4;
	case rank_5: return 5;
	case rank_6: return 6;
	case rank_7: return 7;
	case rank_8: return 8;
	case rank_9: return 9;
	case rank_10: return 10;
	case rank_jack: return 10;
	case rank_queen: return 10;
	case rank_king: return 10;
	case rank_ace: return 11;
	default:
		assert(false && "card rank is invalid");
		return 0;
	}
}

void shuffleDeck(Deck& deck)
{
	// mt is static so it only gets seeded once.
	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

	std::shuffle(deck.begin(), deck.end(), mt);
}

int getSum(const std::vector<Card>& player_card)
{
	int sum{};
	for (const Card& card : player_card)
		sum += getCardValue(card);
	return sum;
}

bool playBlackJack(const Deck& sd)
{
	std::vector <Card> player_cards;
	std::vector <Card> dealer_cards;
	dealer_cards.push_back(sd[0]);
	player_cards.push_back(sd[1]);
	player_cards.push_back(sd[2]);

	int player_sum = getSum(player_cards);
	int dealer_sum = getSum(dealer_cards);

	std::cout << "the player has the cards ";
	printCard(player_cards[0]);
	printCard(player_cards[1]);
	std::cout << "and they total " << player_sum << '\n';

	std::cout << "the dealer has the cards ";
	printCard(dealer_cards[0]);
	std::cout << "and they total " << dealer_sum << '\n';

	int i = 2;
	while (player_sum < 21)
	{
		std::cout << "hit or stand? ";
		std::string choice{};
		std::cin >> choice;
		if (choice == "hit")
			player_cards.push_back(sd[i + 1]);
		else
			break;
		if (player_cards[i].rank == rank_ace && player_sum > 12)
			++player_sum;
		else
			player_sum = getSum(player_cards);
		std::cout << "you drew one more card and now your cards are ";
		for (const Card& card : player_cards)
			printCard(card);
		std::cout << "and they total " << player_sum << '\n';
		++i;
	}
	if (player_sum > 21)
	{
		std::cout << "you bust and lost immediately\n";
		return false;
	}
	else if (player_sum == 21)
	{
		std::cout << "you won\n";
		return true;
	}

	i = 1;
	while (dealer_sum < 17)
	{
		dealer_cards.push_back(sd[i + 3]);
		if (dealer_cards[i].rank == rank_ace && dealer_sum > 12)
			++dealer_sum;
		else
			dealer_sum = getSum(dealer_cards);
		std::cout << "the dealer drew one more card and now the dealer's cards are ";
		for (const Card& card : dealer_cards)
			printCard(card);
		std::cout << "and they total " << dealer_sum << '\n';
		++i;
	}
	std::cout << "the dealer has the total of " << dealer_sum << '\n';
	if (dealer_sum > 21 || dealer_sum < player_sum)
	{
		std::cout << "you won\n";
		return true;
	}
	else if (dealer_sum == player_sum)
	{
		std::cout << "you and the dealer both won.\n";
		return true;
	}
	else
	{
		std::cout << "the dealer won!\n";
		return false;
	}
}

int main()
{
	std::array <Card, 52>deckCard{ createDeck() };

	std::string checker{};
	do
	{
		shuffleDeck(deckCard);
		playBlackJack(deckCard);
		std::cout << "\n\ndo you want to play blackjack again? yes or no? ";
		std::cin >> checker;
	} while (checker == "yes");
}
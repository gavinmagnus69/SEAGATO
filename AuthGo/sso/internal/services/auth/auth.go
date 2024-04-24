package auth

import (
	"context"
	"errors"
	"fmt"
	"golang.org/x/crypto/bcrypt"
	"log/slog"
	"sso/internal/domain/models"
	"sso/internal/lib/jwt"
	"sso/internal/storage"
	"time"
)

type Auth struct {
	log          *slog.Logger
	usrSaver     UserSaver
	userProvider UserProvider
	appProvider  AppProvider
	tokentTTL    time.Duration
}

type UserSaver interface {
	SaveUser(
		ctx context.Context,
		email string,
		passHash []byte) (uid int64, err error)
}

type UserProvider interface {
	User(ctx context.Context, email string) (models.User, error)
	IsAdmin(ctx context.Context, userID int64) (bool, error)
}

type AppProvider interface {
	App(ctx context.Context, appID int) (models.App, error)
}

// New returns a new instance of the Auth service.
func New(log *slog.Logger,
	userSaver UserSaver,
	userProvider UserProvider,
	appProvider AppProvider,
	tokenTTL time.Duration) *Auth {
	return &Auth{
		usrSaver:     userSaver,
		userProvider: userProvider,
		log:          log,
		appProvider:  appProvider,
		tokentTTL:    tokenTTL,
	}
}

var (
	ErrInvalidCredentials = errors.New("Invalid credentials")
	ErrInvalidAppId       = errors.New("Invalid AppID")
	ErrUserExists         = errors.New("User already exists")
)

func (a *Auth) Login(
	ctx context.Context,
	email string,
	password string,
	appID int,
) (string, error) {
	//panic("not implemented")
	const op = "auth.Login"
	log := a.log.With(
		slog.String("op", op),
		slog.String("email", email))
	log.Info("attempting to log in user")
	user, err := a.userProvider.User(ctx, email)
	if err != nil {
		if errors.Is(err, storage.ErrUserNotFound) {
			a.log.Warn("user not found")
			return "", fmt.Errorf("%s: %w", op, ErrInvalidCredentials)
		}

		a.log.Error("failed to get user")
		return "", fmt.Errorf("%s: %w", op, err)
	}

	if err := bcrypt.CompareHashAndPassword(user.PassHash, []byte(password)); err != nil {
		a.log.Info("invalid credentials")
		return "", fmt.Errorf("%s: %w", ErrInvalidCredentials)
	}

	app, err := a.appProvider.App(ctx, appID)

	if err != nil {
		return "", fmt.Errorf("%s: %w", op, err)
	}

	log.Info("user logged in successfully")

	token, err := jwt.NewToken(user, app, a.tokentTTL)
	if err != nil {
		a.log.Error("failed to generate token")
		return "", fmt.Errorf("%s: %w", op, err)
	}

	return token, nil

}

func (a *Auth) RegisterNewUser(
	ctx context.Context,
	email string,
	password string,
) (int64, error) {
	//panic("not implemented")
	const op = "auth.RegisterNewUser"
	log := a.log.With(
		slog.String("op", op),
		slog.String("email", email))

	log.Info("registering user")
	passHash, err := bcrypt.GenerateFromPassword([]byte(password), bcrypt.DefaultCost)
	if err != nil {
		log.Error("failed to generate pass hash")
		return 0, fmt.Errorf("%s: %w", op, err)
	}

	id, err := a.usrSaver.SaveUser(ctx, email, passHash)
	if err != nil {
		if errors.Is(err, storage.ErrUserExists) {
			log.Warn("user already exists")
			return 0, fmt.Errorf("%s: %w", op, ErrUserExists)

		}
		log.Error("failed to save user")
		return 0, fmt.Errorf("%s: %w", op, err)
	}

	log.Info("user registered")

	return id, nil

}

func (a *Auth) IsAdmin(ctx context.Context, userID int64) (bool, error) {
	//panic("not implemented")
	const op = "Auth.IsAdmin"

	log := a.log.With(
		slog.String("op", op),
		slog.Int64("user_id", userID))

	log.Info("checking if user is admin")

	isAdmin, err := a.userProvider.IsAdmin(ctx, userID)
	if err != nil {
		if errors.Is(err, storage.ErrAppNotFound) {
			log.Warn("user not found")

			return false, fmt.Errorf("%s: %w", op, ErrInvalidAppId)
		}
		return false, fmt.Errorf("%s: %w", op, err)
	}

	log.Info("checked if user is admin")

	return isAdmin, nil
}
